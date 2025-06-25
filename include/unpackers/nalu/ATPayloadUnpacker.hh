#ifndef ATPAYLOADUNPACKER_HH
#define ATPAYLOADUNPACKER_HH

#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/nalu/NaluTimeParser.hh"
#include <data_products/nalu/NaluTime.hh>

namespace unpackers {

    class ATPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        ATPayloadUnpacker();

        //Destructor
        ~ATPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluTimePtrCol_;

    private:
        const std::string className_ = "ATPayloadUnpacker";

        //Parsers
        std::unique_ptr<parsers::NaluTimeParser> naluTimeParser_;

    };
}

#endif // ATPAYLOADUNPACKER_HH