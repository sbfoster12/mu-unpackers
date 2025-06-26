#ifndef NALU_TIMEPARSER_HH
#define NALU_TIMEPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/Time.hh>

namespace unpackers::nalu {

    class TimeParser : public unpackers::common::Parser {

    public:
        
        //Constructor
        TimeParser();

        //Destructor
        ~TimeParser();

        //Method to create shared ptr
        std::unique_ptr<data_products::nalu::Time> NewDataProduct();

        //Get methods
        uint64_t    GetCollectionCycleIndex() const;
        uint64_t    GetCollectionCycleTimeStampNs() const;
        double      GetUDPTime() const;
        double      GetParseTime() const;
        double      GetEventTime() const;
        double      GetTotalTime() const;
        uint64_t    GetDataProcessed() const;
        double      GetDataRate() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "TimeParser";

        unpackers::common::DataLocation collection_cycle_index_data_location_;
        unpackers::common::DataLocation collection_cycle_time_stamp_ns_data_location_;
        unpackers::common::DataLocation udp_time_data_location_;
        unpackers::common::DataLocation parse_time_data_location_;
        unpackers::common::DataLocation event_time_data_location_;
        unpackers::common::DataLocation total_time_data_location_;
        unpackers::common::DataLocation data_processed_data_location_;
        unpackers::common::DataLocation data_rate_data_location_;

    };
}

#endif // NALU_TIMEPARSER_HH