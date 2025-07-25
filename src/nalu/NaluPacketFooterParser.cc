#include "unpackers/nalu/NaluPacketFooterParser.hh"

using namespace unpackers;
using unpackers::LoggerHolder;
using json = nlohmann::json;

NaluPacketFooterParser::NaluPacketFooterParser() : unpackers::Parser() {

    //Set each data location to its default values
    parser_index_data_location_ =             unpackers::DataLocation(0,32,2);
    packet_footer_data_location_ =            unpackers::DataLocation(0,48,2);

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
            const auto& nalu_packet_footer = GetSection(j, "nalu_packet_footer");

            parser_index_data_location_ = ParseJsonData(GetSection(nalu_packet_footer, "parser_index"));
            packet_footer_data_location_ = ParseJsonData(GetSection(nalu_packet_footer, "packet_footer"));
        }
    }

}

NaluPacketFooterParser::~NaluPacketFooterParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluPacketFooter> 
NaluPacketFooterParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluPacketFooter>(
                GetParserIndex(),
                GetPacketFooter()
            );
}

//Get methods
uint32_t        NaluPacketFooterParser::GetParserIndex()           const { return GetData(parser_index_data_location_); }
uint32_t        NaluPacketFooterParser::GetPacketFooter()          const { return GetData(packet_footer_data_location_); }

std::ostringstream NaluPacketFooterParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering PacketFooter:" << std::endl;
    oss << "            ParserIndex: " << std::dec << GetParserIndex() << std::endl;
    oss << "            PacketFooter: " << std::hex << "0x" << GetPacketFooter() << std::endl;
    return oss;
}

void NaluPacketFooterParser::Print() {
    std::cout << this->Stream().str();

}
