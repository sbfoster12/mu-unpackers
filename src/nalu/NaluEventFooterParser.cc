#include "unpackers/nalu/NaluEventFooterParser.hh"

using namespace unpackers;
using unpackers::LoggerHolder;
using json = nlohmann::json;

NaluEventFooterParser::NaluEventFooterParser() : unpackers::Parser() {

    //Set each data location to its default values
    event_footer_data_location_ =             unpackers::DataLocation(0,0,2);

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

NaluEventFooterParser::~NaluEventFooterParser() {};

// Method to create the data product
std::unique_ptr<dataProducts::NaluEventFooter> 
NaluEventFooterParser::NewDataProduct() {
    return std::make_unique<dataProducts::NaluEventFooter>(
                GetEventFooter()
            );
}

//Get methods
uint32_t NaluEventFooterParser::GetEventFooter()         const { return GetData(event_footer_data_location_); }

std::ostringstream NaluEventFooterParser::Stream() {
    std::ostringstream oss;
    oss << "   ---> Entering EventFooter: " << std::endl;
    oss << "            EventFooter: " << std::hex << "0x" << GetEventFooter() << std::endl;
    return oss;
}

void NaluEventFooterParser::Print() {
    std::cout << this->Stream().str();

}
