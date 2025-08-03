#ifndef WFD5_CRBANKUNPACKER_HH
#define WFD5_CRBANKUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/BankUnpacker.hh"
#include "unpackers/wfd5/WFD5PayloadUnpacker.hh"
#include "unpackers/fc7/FC7PayloadUnpacker.hh"

#define WFD5_BOARD_TYPE 1
#define FC7_BOARD_TYPE 2

namespace unpackers {

    class WFD5CRBankUnpacker : public BankUnpacker {
        
    public:
        
        //Constructor
        WFD5CRBankUnpacker();

        //Destructor
        ~WFD5CRBankUnpacker();

        // Virtual function to unpack the event
        unpackingStatus UnpackBank(TMEvent* event, const std::string& bankName) override;

        unpackingStatus UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5CRBankUnpacker,1)


    private:
        //class name
        const std::string className_;
    protected:
    };

}

#endif // WFD5_CRBANKUNPACKER_HH