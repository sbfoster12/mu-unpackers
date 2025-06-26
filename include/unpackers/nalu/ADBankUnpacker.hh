#ifndef NALU_ADBANKUNPACKER_HH
#define NALU_ADBANKUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/common/BankUnpacker.hh"
#include "unpackers/nalu/ADPayloadUnpacker.hh"

#define AD_PAYLOAD_ID 0

namespace unpackers::nalu {

    class ADBankUnpacker : public unpackers::common::BankUnpacker {
        
    public:
        
        //Constructor
        ADBankUnpacker();

        //Destructor
        ~ADBankUnpacker();

        // Virtual function to unpack the event
        int UnpackBank(TMEvent* event, const std::string& bankName) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;

        /// ROOT class definition macro with versioning
        ClassDefOverride(ADBankUnpacker,1)

    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // NALU_ADBANKUNPACKER_HH