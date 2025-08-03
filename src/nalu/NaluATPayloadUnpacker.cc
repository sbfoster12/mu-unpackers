#include "unpackers/nalu/NaluATPayloadUnpacker.hh"

using namespace unpackers;
using unpackers::LoggerHolder;

NaluATPayloadUnpacker::NaluATPayloadUnpacker() 
    : unpackers::PayloadUnpacker()
    , naluTimePtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , naluTimeParser_(std::make_unique<NaluTimeParser>())
{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the " << className_ << std::endl;

    //Register the collections
    this->RegisterCollection("NaluTimeCollection",naluTimePtrCol_);

}

NaluATPayloadUnpacker::~NaluATPayloadUnpacker() {};

unpackingStatus NaluATPayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    LoggerHolder::getInstance().InfoLogger << "  We are unpacking an AT payload." << std::endl;

    // Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    // Get the time words
    std::vector<uint64_t> time_words = unpackers::GetXWords(words,wordNum,8,"le");

    // Set the words for the parser
    naluTimeParser_->SetWords(time_words);
    LoggerHolder::getInstance().DebugLogger << naluTimeParser_->Stream().str() << std::endl;

    // Create the data product
    naluTimePtrCol_->push_back(naluTimeParser_->NewDataProduct());

    // Clear data from parser
    naluTimeParser_->Clear();

    return unpackingStatus::Success;
};
