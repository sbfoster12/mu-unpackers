#ifndef WFD5EVENTUNPACKER_HH
#define WFD5EVENTUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/EventUnpacker.hh"
#include "unpackers/wfd5/CRBankUnpacker.hh"

namespace unpackers::wfd5 {

    class WFD5EventUnpacker : public unpackers::common::EventUnpacker {

    public:
        
        //Constructor
        WFD5EventUnpacker();

        //Destructor
        ~WFD5EventUnpacker();

        int UnpackEvent(TMEvent* event) override;

        int UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5EventUnpacker,1)

    private:
        //class name
        const std::string className_;
        
    };

}

#endif // WFD5EVENTUNPACKER_HH