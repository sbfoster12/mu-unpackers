#ifndef FC7PAYLOADUNPACKER_HH
#define FC7PAYLOADUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/fc7/FC7HeaderParser.hh"
#include <data_products/fc7/FC7Header.hh>

namespace unpackers {

    class FC7PayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        FC7PayloadUnpacker();

        //Destructor
        ~FC7PayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> FC7HeaderPtrCol_;

        // ROOT class definition macro with versioning
        ClassDefOverride(FC7PayloadUnpacker,1)

    private:
        const std::string className_ = "FC7PayloadUnpacker";

        //Parser
        std::unique_ptr<unpackers::FC7HeaderParser> FC7HeaderParser_;


    };
}

#endif // FC7PAYLOADUNPACKER_HH