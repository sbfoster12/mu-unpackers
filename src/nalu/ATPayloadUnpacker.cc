#include "unpackers/nalu/ATPayloadUnpacker.hh"

using namespace unpackers::nalu;
using unpackers::common::LoggerHolder;

ATPayloadUnpacker::ATPayloadUnpacker() 
    : unpackers::common::PayloadUnpacker()
    , timePtrCol_(std::make_shared<data_products::common::DataProductPtrCollection>())
    , timeParser_(std::make_unique<TimeParser>())
{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the " << className_ << std::endl;

    //Register the collections
    this->RegisterCollection("TimeCollection",timePtrCol_);

}

ATPayloadUnpacker::~ATPayloadUnpacker() {};

int ATPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    LoggerHolder::getInstance().InfoLogger << "  We are unpacking an AT payload." << std::endl;

    // Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    // Get the time words
    std::vector<uint64_t> time_words = unpackers::common::GetXWords(words,wordNum,8,"le");

    // Set the words for the parser
    timeParser_->SetWords(time_words);
    LoggerHolder::getInstance().DebugLogger << timeParser_->Stream().str() << std::endl;

    // Create the data product
    timePtrCol_->push_back(timeParser_->NewDataProduct());

    // Clear data from parser
    timeParser_->Clear();

    return UNPACKING_SUCCESS;
};
