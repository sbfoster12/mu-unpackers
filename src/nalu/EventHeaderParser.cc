#include "unpackers/nalu/EventHeaderParser.hh"

using namespace unpackers::nalu;
using unpackers::common::LoggerHolder;
using json = nlohmann::json;

EventHeaderParser::EventHeaderParser() : unpackers::common::Parser() {

    event_header_data_location_ =                         unpackers::common::DataLocation(0,0,2);
    event_info_data_location_ =                           unpackers::common::DataLocation(0,16,1);
    event_index_data_location_ =                          unpackers::common::DataLocation(0,24,4);
    event_reference_time_data_location_ =                 unpackers::common::DataLocation(0,56,1,1,0,3);
    packet_size_data_location_ =                          unpackers::common::DataLocation(1,24,2);
    channel_mask_data_location_ =                         unpackers::common::DataLocation(1,40,3,2,0,5);
    num_windows_data_location_ =                          unpackers::common::DataLocation(2,40,1);
    num_packets_data_location_ =                          unpackers::common::DataLocation(2,48,2);

    //Now look for a configuration file and use that

    // Get the path to the data configuration file
    // First get the unpackers path
    const char* unpackers_path = std::getenv("UNPACKERS_PATH");
    if (!unpackers_path) {
        LoggerHolder::getInstance().WarningLogger << "In " << className_ << " UNPACKERS_PATH not set! Using default values for data location instead." << std::endl;;
    } else {

        std::string base_path(unpackers_path);
        std::string file_name = "config/nalu_data_config.json";

        // Make sure base_path ends with a slash '/' or add one
        if (!base_path.empty() && base_path.back() != '/') {
            base_path += '/';
        }

        std::string full_path = base_path + file_name;

        std::ifstream file(full_path);
        if (!file) { 
            LoggerHolder::getInstance().WarningLogger << "In " << className_ << " could not open config file here: " << full_path << ". Using default values for data location instead." << std::endl;
        } else {

            // Make a json object from the input file
            json j;
            file >> j;

            // Access the top-level section first
            const auto& nalu_event_header = GetSection(j, "nalu_event_header");

            event_header_data_location_ = ParseJsonData(GetSection(nalu_event_header, "event_header"));
            event_info_data_location_ = ParseJsonData(GetSection(nalu_event_header, "event_info"));
            event_index_data_location_ = ParseJsonData(GetSection(nalu_event_header, "event_index"));
            event_reference_time_data_location_ = ParseJsonData(GetSection(nalu_event_header, "event_reference_time"));
            packet_size_data_location_ = ParseJsonData(GetSection(nalu_event_header, "packet_size"));
            channel_mask_data_location_ = ParseJsonData(GetSection(nalu_event_header, "channel_mask"));
            num_windows_data_location_ = ParseJsonData(GetSection(nalu_event_header, "num_windows"));
            num_packets_data_location_ = ParseJsonData(GetSection(nalu_event_header, "num_packets"));
        }
    }

};

EventHeaderParser::~EventHeaderParser() {};

// Method to create the data product
std::unique_ptr<data_products::nalu::EventHeader> 
EventHeaderParser::NewDataProduct() {
    return std::make_unique<data_products::nalu::EventHeader>(
                 GetEventHeader()
                ,GetEventInfo()
                ,GetEventIndex()
                ,GetEventReferenceTime()
                ,GetPacketSize()
                ,GetChannelMask()
                ,GetNumWindows()
                ,GetNumPackets()
            );
}

//Get methods
uint16_t EventHeaderParser::GetEventHeader()         const { return GetData(event_header_data_location_); }
uint16_t EventHeaderParser::GetEventInfo()           const { return GetData(event_info_data_location_); }
uint32_t EventHeaderParser::GetEventIndex()          const { return GetData(event_index_data_location_); }
uint32_t EventHeaderParser::GetEventReferenceTime()  const { return GetData(event_reference_time_data_location_); }
uint16_t EventHeaderParser::GetPacketSize()          const { return GetData(packet_size_data_location_); }
uint64_t EventHeaderParser::GetChannelMask()         const { return GetData(channel_mask_data_location_); }
uint16_t EventHeaderParser::GetNumWindows()          const { return GetData(num_windows_data_location_); }
uint16_t EventHeaderParser::GetNumPackets()          const { return GetData(num_packets_data_location_); }

std::ostringstream EventHeaderParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering EventHeader: " << std::endl;
    oss << "            EventHeader: " << "0x" << std::hex << std::setw(4) << std::setfill('0') << GetEventHeader() << std::endl;
    oss << "            EventInfo: " << std::dec << GetEventInfo() << std::endl;
    oss << "            EventIndex: "<< std::dec << GetEventIndex() << std::endl;
    oss << "            EventReferenceTime: " << std::dec << GetEventReferenceTime() << std::endl;
    oss << "            PacketSize: " << std::dec << GetPacketSize() << std::endl;
    oss << "            ChannelMask: " << "0x" <<  std::hex << std::setw(16) << std::setfill('0') << GetChannelMask() << std::endl;
    oss << "            NumWindows: " << std::dec << GetNumWindows() << std::endl;
    oss << "            NumPackets: " << std::dec << GetNumPackets() << std::endl;
    return oss;
}

void EventHeaderParser::Print() {
    std::cout << this->Stream().str();

}
