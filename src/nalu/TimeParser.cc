#include "unpackers/nalu/TimeParser.hh"

using namespace unpackers::nalu;
using unpackers::common::LoggerHolder;
using json = nlohmann::json;

TimeParser::TimeParser() : unpackers::common::Parser() {

    //Set each data location to its default values
    collection_cycle_index_data_location_ =             unpackers::common::DataLocation(0,0,8);
    collection_cycle_time_stamp_ns_data_location_ =     unpackers::common::DataLocation(1,0,8);
    udp_time_data_location_ =                           unpackers::common::DataLocation(2,0,8);
    parse_time_data_location_ =                         unpackers::common::DataLocation(3,0,8);
    event_time_data_location_ =                         unpackers::common::DataLocation(4,0,8);
    total_time_data_location_ =                         unpackers::common::DataLocation(5,0,8);
    data_processed_data_location_ =                     unpackers::common::DataLocation(6,0,8);
    data_rate_data_location_ =                          unpackers::common::DataLocation(7,0,8);

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
            const auto& nalu_time = GetSection(j, "nalu_time");

            collection_cycle_index_data_location_ = ParseJsonData(GetSection(nalu_time, "collection_cycle_index"));
            collection_cycle_time_stamp_ns_data_location_ = ParseJsonData(GetSection(nalu_time, "collection_cycle_time_stamps_ns"));
            udp_time_data_location_ = ParseJsonData(GetSection(nalu_time, "udp_time"));
            parse_time_data_location_ = ParseJsonData(GetSection(nalu_time, "parse_time"));
            event_time_data_location_ = ParseJsonData(GetSection(nalu_time, "event_time"));
            total_time_data_location_ = ParseJsonData(GetSection(nalu_time, "total_time"));
            data_processed_data_location_ = ParseJsonData(GetSection(nalu_time, "data_processed"));
            data_rate_data_location_ = ParseJsonData(GetSection(nalu_time, "data_rate"));
        }
    }
};

TimeParser::~TimeParser() {};

// Method to create the data product
std::unique_ptr<data_products::nalu::Time> 
TimeParser::NewDataProduct() {
    return std::make_unique<data_products::nalu::Time>(
                 GetCollectionCycleIndex()
                ,GetCollectionCycleTimeStampNs()
                ,GetUDPTime()
                ,GetParseTime()
                ,GetEventTime()
                ,GetTotalTime()
                ,GetDataProcessed()
                ,GetDataRate()
            );
}

//Get methods
uint64_t    TimeParser::GetCollectionCycleIndex()           const { return GetData(collection_cycle_index_data_location_); }
uint64_t    TimeParser::GetCollectionCycleTimeStampNs()     const { return GetData(collection_cycle_time_stamp_ns_data_location_); }
double      TimeParser::GetUDPTime()                        const { uint64_t word = GetData(udp_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      TimeParser::GetParseTime()                      const { uint64_t word = GetData(parse_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      TimeParser::GetEventTime()                      const { uint64_t word = GetData(event_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
double      TimeParser::GetTotalTime()                      const { uint64_t word = GetData(total_time_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }
uint64_t    TimeParser::GetDataProcessed()                  const { return GetData(data_processed_data_location_); }
double      TimeParser::GetDataRate()                       const { uint64_t word = GetData(data_rate_data_location_); double result; std::memcpy(&result, &word, sizeof(double)); return result; }


std::ostringstream TimeParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering Time: " << std::endl;
    oss << "            CollectionCycleIndex: " << GetCollectionCycleIndex() << std::endl;
    oss << "            CollectionCycleTimeStampNs: " << GetCollectionCycleTimeStampNs() << std::endl;
    oss << "            UDPTime: " << GetUDPTime() << std::endl;
    oss << "            ParseTime " << GetParseTime() << std::endl;
    oss << "            EventTime: " << GetEventTime() << std::endl;
    oss << "            TotalTime: " << GetTotalTime() << std::endl;
    oss << "            DataProcessed: " << GetDataProcessed() << std::endl;
    oss << "            DataRate: " << GetDataRate() << std::endl;
    return oss;
}

void TimeParser::Print() {
    std::cout << this->Stream().str();

}
