#include "unpackers/wfd5/WFD5EventUnpacker.hh"

using namespace unpackers::wfd5;
using unpackers::common::LoggerHolder;


WFD5EventUnpacker::WFD5EventUnpacker()
    : EventUnpacker(), 
    className_("WFD5EventUnpacker")
{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the basic event unpacker." << std::endl;

    //Create the bank unpackers
    bankUnpackers_[CR_BANK_ID] = std::make_unique<CRBankUnpacker>();

    //Register the collections in the bank unpacker
    for (const auto& bankUnpacker : bankUnpackers_) {
        this->RegisterCollections(bankUnpacker.second->GetCollections());
    }
}

WFD5EventUnpacker::~WFD5EventUnpacker() {}

int WFD5EventUnpacker::UnpackEvent(TMEvent* event) {

    //Clear previous event
    this->ClearCollections();

    LoggerHolder::getInstance().InfoLogger << "Unpacking with Event ID: " << event->event_id << " and SN: " << event->serial_number << std::endl;

    // Loop over all banks and read the CR banks
    // Can we parallelize this loop?
    event->FindAllBanks();
    for (const auto& bank : event->banks) {
        if (bank.name.substr(0, 2) == "CR") {
            auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(event, bank.name);

            if (status == UNPACKING_FAILURE) {
                //Something went wrong, so clear the collections and return failure
                this->ClearCollections();
                return UNPACKING_FAILURE;
            }
        }
    }

    return UNPACKING_SUCCESS;
}

//method to unpack a bank directly instead of the event
int WFD5EventUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) {
    if (bankName.substr(0, 2) == "CR") {
        bankUnpackers_[CR_BANK_ID]->ClearCollections();
        int crateNum = std::stoi(bankName.substr(3, 4));
        auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(bankData, totalWords, serialNumber, crateNum);
        if (status == UNPACKING_FAILURE) {
            //Something went wrong, so clear the collections and return failure
            bankUnpackers_[CR_BANK_ID]->ClearCollections();
            return UNPACKING_FAILURE;
        }
    }
    return UNPACKING_SUCCESS;
}