#ifndef NALU_EVENTUNPACKER_HH
#define NALU_EVENTUNPACKER_HH

//Custom
#include "unpackers/common/Logger.hh"
#include "unpackers/common/EventUnpacker.hh"
#include "unpackers/nalu/NaluADBankUnpacker.hh"
#include "unpackers/nalu/NaluATBankUnpacker.hh"

#define AD_BANK_ID 0
#define AT_BANK_ID 1
    
namespace unpackers {

    class NaluEventUnpacker : public EventUnpacker {
        
    public:
        
        //Constructor
        NaluEventUnpacker();

        //Destructor
        ~NaluEventUnpacker();

        unpackingStatus UnpackEvent(TMEvent* event) override;

        unpackingStatus UnpackBank(uint64_t* bank_data, unsigned int total_words, int serial_number, std::string bank_name) override;

        /// ROOT class definition macro with versioning
        ClassDefOverride(NaluEventUnpacker,1)

    private:
        //class name
        const std::string className_;
        
    };

}

#endif // NALU_EVENTUNPACKER_HH