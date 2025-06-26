#include "unpackers/nalu/EventFooterParser.hh"

using namespace unpackers::nalu;
using unpackers::common::LoggerHolder;
using json = nlohmann::json;

EventFooterParser::EventFooterParser() : unpackers::common::Parser() {

    //Set each data location to its default values
    event_footer_data_location_ =             unpackers::common::DataLocation(0,0,2);

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
            const auto& nalu_event_footer = GetSection(j, "nalu_event_footer");

            event_footer_data_location_ = ParseJsonData(GetSection(nalu_event_footer, "event_footer"));
        }
    }

};

EventFooterParser::~EventFooterParser() {};

// Method to create the data product
std::unique_ptr<data_products::nalu::EventFooter> 
EventFooterParser::NewDataProduct() {
    return std::make_unique<data_products::nalu::EventFooter>(
                GetEventFooter()
            );
}

//Get methods
uint32_t EventFooterParser::GetEventFooter()         const { return GetData(event_footer_data_location_); }

std::ostringstream EventFooterParser::Stream() {
    std::ostringstream oss;
    oss << "   ---> Entering EventFooter: " << std::endl;
    oss << "            EventFooter: " << std::hex << "0x" << GetEventFooter() << std::endl;
    return oss;
}

void EventFooterParser::Print() {
    std::cout << this->Stream().str();

}
