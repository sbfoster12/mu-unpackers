#ifndef NALU_ATPAYLOADUNPACKER_HH
#define NALU_ATPAYLOADUNPACKER_HH

#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/nalu/TimeParser.hh"
#include <data_products/nalu/Time.hh>

namespace unpackers::nalu {

    class ATPayloadUnpacker : public unpackers::common::PayloadUnpacker {
        
    public:
        
        //Constructor
        ATPayloadUnpacker();

        //Destructor
        ~ATPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<data_products::common::DataProductPtrCollection> timePtrCol_;

        /// ROOT class definition macro with versioning
        ClassDefOverride(ATPayloadUnpacker,1)

    private:
        const std::string className_ = "ATPayloadUnpacker";

        //Parsers
        std::unique_ptr<unpackers::nalu::TimeParser> timeParser_;

    };
}

#endif // NALU_ATPAYLOADUNPACKER_HH