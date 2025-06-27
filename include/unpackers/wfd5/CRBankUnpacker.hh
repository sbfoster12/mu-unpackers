#ifndef CRBANKUNPACKER_HH
#define CRBANKUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/BankUnpacker.hh"
#include "unpackers/wfd5/WFD5PayloadUnpacker.hh"
#include "unpackers/wfd5/FC7PayloadUnpacker.hh"

#define WFD5_BOARD_TYPE 1
#define FC7_BOARD_TYPE 2

namespace unpackers::wfd5 {

    class CRBankUnpacker : public unpackers::common::BankUnpacker {
        
    public:
        
        //Constructor
        CRBankUnpacker();

        //Destructor
        ~CRBankUnpacker();

        // Virtual function to unpack the event
        int UnpackBank(TMEvent* event, const std::string& bankName) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;

        // ROOT class definition macro with versioning
        ClassDefOverride(CRBankUnpacker,1)


    private:
        //class name
        const std::string className_;

    protected:
    };

}

#endif // CRBANKUNPACKER_HH