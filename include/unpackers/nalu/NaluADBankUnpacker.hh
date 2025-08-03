#ifndef NALU_ADBANKUNPACKER_HH
#define NALU_ADBANKUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/common/BankUnpacker.hh"
#include "unpackers/nalu/NaluADPayloadUnpacker.hh"

#define AD_PAYLOAD_ID 0

namespace unpackers {

    class NaluADBankUnpacker : public BankUnpacker {
        
    public:
        
        //Constructor
        NaluADBankUnpacker();

        //Destructor
        ~NaluADBankUnpacker();

        // Virtual function to unpack the event
        unpackingStatus UnpackBank(TMEvent* event, const std::string& bankName) override;

        unpackingStatus UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;

        /// ROOT class definition macro with versioning
        ClassDefOverride(NaluADBankUnpacker,1)

    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // NALU_ADBANKUNPACKER_HH