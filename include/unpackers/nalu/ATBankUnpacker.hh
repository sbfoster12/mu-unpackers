#ifndef NALU_ATBANKUNPACKER_HH
#define NALU_ATBANKUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/common/BankUnpacker.hh"
#include "unpackers/nalu/ATPayloadUnpacker.hh"

#define AT_PAYLOAD_ID 0

namespace unpackers::nalu {

    class ATBankUnpacker : public unpackers::common::BankUnpacker {
        
    public:
        
        //Constructor
        ATBankUnpacker();

        //Destructor
        ~ATBankUnpacker();

        // Virtual function to unpack the event
        int UnpackBank(TMEvent* event, const std::string& bankName) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;

        /// ROOT class definition macro with versioning
        ClassDefOverride(ATBankUnpacker,1)

    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // NALU_ATBANKUNPACKER_HH