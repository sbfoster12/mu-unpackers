#ifndef WFD5_EVENTUNPACKER_HH
#define WFD5_EVENTUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/EventUnpacker.hh"
#include "unpackers/wfd5/WFD5CRBankUnpacker.hh"

namespace unpackers {

    class WFD5EventUnpacker : public EventUnpacker {

    public:
        
        //Constructor
        WFD5EventUnpacker();

        //Destructor
        ~WFD5EventUnpacker();

        unpackingStatus UnpackEvent(TMEvent* event) override;

        unpackingStatus UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5EventUnpacker,1)

    private:
        //class name
        const std::string className_;
        
    };

}

#endif // WFD5_EVENTUNPACKER_HH