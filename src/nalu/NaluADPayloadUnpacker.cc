#include "unpackers/nalu/NaluADPayloadUnpacker.hh"

using namespace unpackers;
using unpackers::LoggerHolder;


NaluADPayloadUnpacker::NaluADPayloadUnpacker() 
    : unpackers::PayloadUnpacker()
    , naluEventHeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluPacketHeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluPacketFooterPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluWaveformPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluEventFooterPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluEventHeaderParser_(std::make_unique<NaluEventHeaderParser>())
    , naluPacketHeaderParser_(std::make_unique<NaluPacketHeaderParser>())
    , naluPacketParser_(std::make_unique<NaluPacketParser>())
    , naluPacketFooterParser_(std::make_unique<NaluPacketFooterParser>())
    , naluEventFooterParser_(std::make_unique<NaluEventFooterParser>())

{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the " << className_ << std::endl;

    //Register the collections
    this->RegisterCollection("NaluEventHeaderCollection",naluEventHeaderPtrCol_);
    this->RegisterCollection("NaluPacketHeaderCollection",naluPacketHeaderPtrCol_);
    this->RegisterCollection("NaluPacketFooterCollection",naluPacketFooterPtrCol_);
    this->RegisterCollection("NaluWaveformCollection",naluWaveformPtrCol_);
    this->RegisterCollection("NaluEventFooterCollection",naluEventFooterPtrCol_);

}

NaluADPayloadUnpacker::~NaluADPayloadUnpacker() {};

int NaluADPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    LoggerHolder::getInstance().InfoLogger << "  We are unpacking an AD payload." << std::endl;

    // Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    // std::cout << "wordNum: " << wordNum << std::endl;

    // Get the event header, which is 3 64 bit words long

    std::vector<uint64_t> header_words = unpackers::GetXWords(words,wordNum,3,"le");
    // for (int i = 0; i < header_words.size(); ++i)
    // {
    //     std::cout << std::hex << std::setw(16) << std::setfill('0') << header_words.at(i) << std::endl;
    // }

    naluEventHeaderParser_->SetWords(header_words);
    LoggerHolder::getInstance().DebugLogger << naluEventHeaderParser_->Stream().str() << std::endl;

    //Create the data product
    naluEventHeaderPtrCol_->push_back(naluEventHeaderParser_->NewDataProduct());

    // Get the event index and other information
    int event_index = naluEventHeaderParser_->GetEventIndex();
    int num_packets = naluEventHeaderParser_->GetNumPackets();
    int num_windows = naluEventHeaderParser_->GetNumWindows();

    // We are now going to loop over the packets, but we need to figure out which packet goes to which channel and the order of the packets

    // Collection of nalu packets, grouped by channel number
    std::map<int,dataProducts::NaluPacketCollection> channel_to_packets_map = {};

    // Loop over each packet to parse and store the data
    for (int i_packet = 0; i_packet < num_packets; ++i_packet)
    {
        // --- HEADER ---

        // Extract 2 64-bit words for the packet header (little-endian)
        std::vector<uint64_t> packet_header_words = unpackers::GetXWords(words, wordNum, 2, "le");

        // Adjust word index because the last header word also contains sample data
        wordNum--;

        // Pass the header words to the header parser
        naluPacketHeaderParser_->SetWords(packet_header_words);
        LoggerHolder::getInstance().DebugLogger << naluPacketHeaderParser_->Stream().str()  << std::endl;

        // Create and store the header data product
        naluPacketHeaderPtrCol_->push_back(naluPacketHeaderParser_->NewDataProduct());

        // Retrieve key metadata from the parsed header
        int channel_num = naluPacketHeaderParser_->GetChannel();
        int window_position = naluPacketHeaderParser_->GetWindowPosition();

        // --- SAMPLES ---

        // Extract 9 64-bit words containing sample data (big-endian)
        std::vector<uint64_t> sample_words = unpackers::GetXWords(words, wordNum, 9, "bigendian");

        // Adjust word index because the last sample word includes the packet footer
        wordNum--;

        // Parse the sample data
        naluPacketParser_->SetWords(sample_words);
        LoggerHolder::getInstance().DebugLogger << naluPacketParser_->Stream().str()  << std::endl;

        // --- STORE PACKET DATA BY CHANNEL ---

        // Ensure there's a vector ready for this channel in the map
        if (channel_to_packets_map.find(channel_num) == channel_to_packets_map.end()) {
            channel_to_packets_map[channel_num] = {};
        }

        // Store the parsed packet (with channel and position metadata) into the map
        channel_to_packets_map[channel_num].push_back(
            naluPacketParser_->NewDataProduct(channel_num, window_position)
        );

        // --- FOOTER ---

        // Extract 1 64-bit word for the packet footer (little-endian)
        std::vector<uint64_t> packet_footer_words = unpackers::GetXWords(words, wordNum, 1, "le");

        // Parse the footer
        naluPacketFooterParser_->SetWords(packet_footer_words);
        LoggerHolder::getInstance().DebugLogger << naluPacketFooterParser_->Stream().str() << std::endl;

        // Create and store the footer data product
        naluPacketFooterPtrCol_->push_back(naluPacketFooterParser_->NewDataProduct());
    }

    // Loop over the channel map to stitch together the packets
    for (const auto& val : channel_to_packets_map) {

        // Sort the packets by window position
        auto packets = val.second;

        std::sort(packets.begin(), packets.end(),
        [](const dataProducts::NaluPacket& a, const dataProducts::NaluPacket& b) {
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
            [pivot_window](const dataProducts::NaluPacket& a, const dataProducts::NaluPacket& b) {
                return ((a.window_position - pivot_window) % 62) < ((b.window_position - pivot_window) % 62);
            });
        }

        // Now stitch together these packets to form a waveform and push to the collection
        naluWaveformPtrCol_->push_back(std::make_unique<dataProducts::NaluWaveform>(packets));

    }

    // Extract 1 64-bit words for the event footer (little-endian)
    std::vector<uint64_t> footer_words = unpackers::GetXWords(words,wordNum,1,"le");
    
    // Pass the footer words to the footer parser
    naluEventFooterParser_->SetWords(footer_words);
    LoggerHolder::getInstance().DebugLogger << naluEventFooterParser_->Stream().str()  << std::endl;

    uint32_t event_footer = naluEventFooterParser_->GetEventFooter();

    // Create and store the event footer data product
    naluEventFooterPtrCol_->push_back(naluEventFooterParser_->NewDataProduct());

    //Check that the footer word is 0xEEEE
    if (event_footer != 0xEEEE) {
        std::cerr << std::hex << "Error: wrong footer word\n"
        << "Location: int NaluADPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum)\n"
        << "Details: The footer word is " << event_footer << ", but should be 0xEEEE"<< std::endl;
        return UNPACKING_FAILURE;
    } else {
        LoggerHolder::getInstance().DebugLogger << "  Reached end of payload with footer word 0x" << std::hex << event_footer << std::endl;
    }

    //Clear data from parser
    naluEventHeaderParser_->Clear();
    naluPacketHeaderParser_->Clear();
    naluPacketParser_->Clear();
    naluPacketFooterParser_->Clear();
    naluEventFooterParser_->Clear();

    return UNPACKING_SUCCESS;
};
