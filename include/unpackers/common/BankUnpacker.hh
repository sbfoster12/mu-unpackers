#ifndef BANKUNPACKER_HH
#define BANKUNPACKER_HH

//Standard
#include <iostream>
#include <string>
#include <map>

//Custom
#include "midasio.h"
#include "unpackers/common/CollectionsHolder.hh"
#include "unpackers/common/PayloadUnpacker.hh"

namespace unpackers {

    class BankUnpacker : public CollectionsHolder {
        
    public:
        
        //Constructor
        BankUnpacker();

        //Destructor
        virtual ~BankUnpacker();

        // Virtual function to read the event
        virtual unpackingStatus UnpackBank(TMEvent* event, const std::string& bankName) = 0;

        virtual unpackingStatus UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, int crateNum) = 0;

        void UpdateEventNum(int eventNum);
        void UpdateCrateNum(int crateNum);

    private:
        //class name
        const std::string className_;

    protected:
        //unpackers
        std::map<int,std::unique_ptr<PayloadUnpacker>> payloadUnpackers_;

    };
}

#endif // BANKUNPACKER_HH