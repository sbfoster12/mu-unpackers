#include "unpackers/wfd5/WFD5EventUnpacker.hh"

using namespace unpackers;
using unpackers::LoggerHolder;


WFD5EventUnpacker::WFD5EventUnpacker()
    : EventUnpacker(), 
    className_("WFD5EventUnpacker")
{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the basic event unpacker." << std::endl;

    //Create the bank unpackers
    bankUnpackers_[CR_BANK_ID] = std::make_unique<WFD5CRBankUnpacker>();

    //Register the collections in the bank unpacker
    for (const auto& bankUnpacker : bankUnpackers_) {
        this->RegisterCollections(bankUnpacker.second->GetCollections());
    }
}

WFD5EventUnpacker::~WFD5EventUnpacker() {}

unpackingStatus WFD5EventUnpacker::UnpackEvent(TMEvent* event) {

    //Check internal state first
    if (currentIteration_ == -1) {
        // We have not processed the event yet

        //Clear previous event
        this->ClearCollections();

        LoggerHolder::getInstance().InfoLogger << "Unpacking with Event ID: " << event->event_id << " and SN: " << event->serial_number << std::endl;

        // Loop over all banks and read the CR banks
        // Can we parallelize this loop?
        event->FindAllBanks();
        for (const auto& bank : event->banks) {
            if (bank.name.substr(0, 2) == "CR") {
                auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(event, bank.name);

                if (status == unpackingStatus::Failure) {
                    //Something went wrong, so clear the collections and return failure
                    this->ClearCollections();
                    return unpackingStatus::Failure;
                }
            }
        }

        // It is possible that multiple trigger events are in this unpacked midas event
        // Initialize iterations for accessing each of these events separately using the waveform index
        this->SetIterations<dataProducts::WFD5Waveform>("WFD5WaveformCollection");
        // for (const auto& [index, iteration] : iterationsMap_) {
        //     std::cout << "Index: " << index << ", Iteration: " << iteration << std::endl;
        // }

    } else { // if currentIteration_ != -1)
        // This is not the first time we are processing this event
        // Increment the iterations
        currentIteration_++;
    }

    // Now return the status
    if (currentIteration_ < maxIterations_) {
       return unpackingStatus::SuccessMore;
    }
    else if (currentIteration_ == maxIterations_) {
        // We have processed all iterations, reset internal state
        this->ResetIterations();
        return unpackingStatus::SuccessDone;
    } else {
        // Something went wrong to get here
        this->ResetIterations();
        return unpackingStatus::Failure;
    }
}

//method to unpack a bank directly instead of the event
unpackingStatus WFD5EventUnpacker::UnpackBank(uint64_t* bankData, unsigned int totalWords, int serialNumber, std::string bankName) {
    
    //Check internal state first
    if (currentIteration_ == -1) {
        // We have not processed this bankData yet
        if (bankName.substr(0, 2) == "CR") {
            bankUnpackers_[CR_BANK_ID]->ClearCollections();
            int crateNum = std::stoi(bankName.substr(3, 4));
            auto status = bankUnpackers_[CR_BANK_ID]->UnpackBank(bankData, totalWords, serialNumber, crateNum);
            if (status == unpackingStatus::Failure) {
                //Something went wrong, so clear the collections and return failure
                bankUnpackers_[CR_BANK_ID]->ClearCollections();
                return unpackingStatus::Failure;
            }
            // Initialize iterations for accessing each of these events separately using the waveform index
            this->SetIterations<dataProducts::WFD5Waveform>("WFD5WaveformCollection");
        }
    } else { // if currentIteration_ != -1)
        // This is not the first time we are processing this event
        // Increment the iterations
        currentIteration_++;
    }

    // Now return the status
    if (currentIteration_ < maxIterations_) {
       return unpackingStatus::SuccessMore;
    }
    else if (currentIteration_ == maxIterations_) {
        // We have processed all iterations, reset internal state
        this->ResetIterations();
        return unpackingStatus::SuccessDone;
    } else {
        // Something went wrong to get here
        this->ResetIterations();
        return unpackingStatus::Failure;
    }

    return unpackingStatus::Success;
}