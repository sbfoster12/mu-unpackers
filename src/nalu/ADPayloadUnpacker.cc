#include "unpackers/nalu/ADPayloadUnpacker.hh"

using namespace unpackers::nalu;
using unpackers::common::LoggerHolder;


ADPayloadUnpacker::ADPayloadUnpacker() 
    : unpackers::common::PayloadUnpacker()
    , eventHeaderPtrCol_(std::make_shared<data_products::common::DataProductPtrCollection>())
    , packetHeaderPtrCol_(std::make_shared<data_products::common::DataProductPtrCollection>())
    , packetFooterPtrCol_(std::make_shared<data_products::common::DataProductPtrCollection>())
    , waveformPtrCol_(std::make_shared<data_products::common::DataProductPtrCollection>())
    , eventFooterPtrCol_(std::make_shared<data_products::common::DataProductPtrCollection>())
    , eventHeaderParser_(std::make_unique<EventHeaderParser>())
    , packetHeaderParser_(std::make_unique<PacketHeaderParser>())
    , packetParser_(std::make_unique<PacketParser>())
    , packetFooterParser_(std::make_unique<PacketFooterParser>())
    , eventFooterParser_(std::make_unique<EventFooterParser>())

{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the " << className_ << std::endl;

    //Register the collections
    this->RegisterCollection("EventHeaderCollection",eventHeaderPtrCol_);
    this->RegisterCollection("PacketHeaderCollection",packetHeaderPtrCol_);
    this->RegisterCollection("PacketFooterCollection",packetFooterPtrCol_);
    this->RegisterCollection("WaveformCollection",waveformPtrCol_);
    this->RegisterCollection("EventFooterCollection",eventFooterPtrCol_);

}

ADPayloadUnpacker::~ADPayloadUnpacker() {};

int ADPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    LoggerHolder::getInstance().InfoLogger << "  We are unpacking an AD payload." << std::endl;

    // Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    // std::cout << "wordNum: " << wordNum << std::endl;

    // Get the event header, which is 3 64 bit words long

    std::vector<uint64_t> header_words = unpackers::common::GetXWords(words,wordNum,3,"le");
    // for (int i = 0; i < header_words.size(); ++i)
    // {
    //     std::cout << std::hex << std::setw(16) << std::setfill('0') << header_words.at(i) << std::endl;
    // }

    eventHeaderParser_->SetWords(header_words);
    LoggerHolder::getInstance().DebugLogger << eventHeaderParser_->Stream().str() << std::endl;

    //Create the data product
    eventHeaderPtrCol_->push_back(eventHeaderParser_->NewDataProduct());

    // Get the event index and other information
    int event_index = eventHeaderParser_->GetEventIndex();
    int num_packets = eventHeaderParser_->GetNumPackets();
    int num_windows = eventHeaderParser_->GetNumWindows();

    // We are now going to loop over the packets, but we need to figure out which packet goes to which channel and the order of the packets

    // Collection of nalu packets, grouped by channel number
    std::map<int,data_products::nalu::PacketCollection> channel_to_packets_map = {};

    // Loop over each packet to parse and store the data
    for (int i_packet = 0; i_packet < num_packets; ++i_packet)
    {
        // --- HEADER ---

        // Extract 2 64-bit words for the packet header (little-endian)
        std::vector<uint64_t> packet_header_words = unpackers::common::GetXWords(words, wordNum, 2, "le");

        // Adjust word index because the last header word also contains sample data
        wordNum--;

        // Pass the header words to the header parser
        packetHeaderParser_->SetWords(packet_header_words);
        LoggerHolder::getInstance().DebugLogger << packetHeaderParser_->Stream().str()  << std::endl;

        // Create and store the header data product
        packetHeaderPtrCol_->push_back(packetHeaderParser_->NewDataProduct());

        // Retrieve key metadata from the parsed header
        int channel_num = packetHeaderParser_->GetChannel();
        int window_position = packetHeaderParser_->GetWindowPosition();

        // --- SAMPLES ---

        // Extract 9 64-bit words containing sample data (big-endian)
        std::vector<uint64_t> sample_words = unpackers::common::GetXWords(words, wordNum, 9, "bigendian");

        // Adjust word index because the last sample word includes the packet footer
        wordNum--;

        // Parse the sample data
        packetParser_->SetWords(sample_words);
        LoggerHolder::getInstance().DebugLogger << packetParser_->Stream().str()  << std::endl;

        // --- STORE PACKET DATA BY CHANNEL ---

        // Ensure there's a vector ready for this channel in the map
        if (channel_to_packets_map.find(channel_num) == channel_to_packets_map.end()) {
            channel_to_packets_map[channel_num] = {};
        }

        // Store the parsed packet (with channel and position metadata) into the map
        channel_to_packets_map[channel_num].push_back(
            packetParser_->NewDataProduct(channel_num, window_position)
        );

        // --- FOOTER ---

        // Extract 1 64-bit word for the packet footer (little-endian)
        std::vector<uint64_t> packet_footer_words = unpackers::common::GetXWords(words, wordNum, 1, "le");

        // Parse the footer
        packetFooterParser_->SetWords(packet_footer_words);
        LoggerHolder::getInstance().DebugLogger << packetFooterParser_->Stream().str() << std::endl;

        // Create and store the footer data product
        packetFooterPtrCol_->push_back(packetFooterParser_->NewDataProduct());
    }

    // Loop over the channel map to stitch together the packets
    for (const auto& val : channel_to_packets_map) {

        // Sort the packets by window position
        auto packets = val.second;

        std::sort(packets.begin(), packets.end(),
        [](const data_products::nalu::Packet& a, const data_products::nalu::Packet& b) {
            return a.window_position < b.window_position;
        });

        // Check if there is a wrap around (e.g. 60,61,0,1) and find the pivot window
        int pivot_window = -1;
        if (packets.size() > 1) {

            for (size_t i = 0; i < packets.size() - 1; ++i) {
                if (packets[i].window_position + 1 != packets[i + 1].window_position) {
                    pivot_window = packets[i + 1].window_position;
                }
            }
        }

        // Use pivot window to resort packets
        if (pivot_window != -1) {
            std::sort(packets.begin(), packets.end(),
            [pivot_window](const data_products::nalu::Packet& a, const data_products::nalu::Packet& b) {
                return ((a.window_position - pivot_window) % 62) < ((b.window_position - pivot_window) % 62);
            });
        }

        // Now stitch together these packets to form a waveform and push to the collection
        waveformPtrCol_->push_back(std::make_unique<data_products::nalu::Waveform>(packets));

    }

    // Extract 1 64-bit words for the event footer (little-endian)
    std::vector<uint64_t> footer_words = unpackers::common::GetXWords(words,wordNum,1,"le");
    
    // Pass the footer words to the footer parser
    eventFooterParser_->SetWords(footer_words);
    LoggerHolder::getInstance().DebugLogger << eventFooterParser_->Stream().str()  << std::endl;

    uint32_t event_footer = eventFooterParser_->GetEventFooter();

    // Create and store the event footer data product
    eventFooterPtrCol_->push_back(eventFooterParser_->NewDataProduct());

    //Check that the footer word is 0xEEEE
    if (event_footer != 0xEEEE) {
        std::cerr << std::hex << "Error: wrong footer word\n"
        << "Location: int ADPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum)\n"
        << "Details: The footer word is " << event_footer << ", but should be 0xEEEE"<< std::endl;
        return UNPACKING_FAILURE;
    } else {
        LoggerHolder::getInstance().DebugLogger << "  Reached end of payload with footer word 0x" << std::hex << event_footer << std::endl;
    }

    //Clear data from parser
    eventHeaderParser_->Clear();
    packetHeaderParser_->Clear();
    packetParser_->Clear();
    packetFooterParser_->Clear();
    eventFooterParser_->Clear();

    return UNPACKING_SUCCESS;
};
