#ifndef NALU_ATPAYLOADUNPACKER_HH
#define NALU_ATPAYLOADUNPACKER_HH

#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/nalu/NaluTimeParser.hh"
#include <data_products/nalu/NaluTime.hh>

namespace unpackers {

    class NaluATPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        NaluATPayloadUnpacker();

        //Destructor
        ~NaluATPayloadUnpacker();

        unpackingStatus Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluTimePtrCol_;

        /// ROOT class definition macro with versioning
        ClassDefOverride(NaluATPayloadUnpacker,1)

    private:
        const std::string className_ = "NaluATPayloadUnpacker";

        //Parsers
        std::unique_ptr<unpackers::NaluTimeParser> naluTimeParser_;

    };
}

#endif // NALU_ATPAYLOADUNPACKER_HH