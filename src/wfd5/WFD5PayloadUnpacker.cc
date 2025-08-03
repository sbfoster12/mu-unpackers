#include "unpackers/wfd5/WFD5PayloadUnpacker.hh"

using namespace unpackers;
using unpackers::LoggerHolder;

WFD5PayloadUnpacker::WFD5PayloadUnpacker() 
    : PayloadUnpacker()
    , WFD5HeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WFD5ChannelHeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WFD5WaveformHeaderPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WFD5WaveformPtrCol_(std::make_shared<dataProducts::DataProductPtrCollection>())
    , WFD5HeaderParser_(std::make_unique<unpackers::WFD5HeaderParser>())
    , WFD5ChannelHeaderParser_(std::make_unique<unpackers::WFD5ChannelHeaderParser>())
    , WFD5WaveformHeaderParser_(std::make_unique<unpackers::WFD5WaveformHeaderParser>())
    , WFD5WaveformParser_(std::make_unique<unpackers::WFD5WaveformParser>())
    , WFD5ChannelHeaderAsyncParser_(std::make_unique<unpackers::WFD5ChannelHeaderAsyncParser>())
    , WFD5WaveformHeaderAsyncParser_(std::make_unique<unpackers::WFD5WaveformHeaderAsyncParser>())
{
    LoggerHolder::getInstance().InfoLogger << "We are constructing the WFD5 unpacker." << std::endl;

    //Register the collections
    this->RegisterCollection("WFD5HeaderCollection",WFD5HeaderPtrCol_);
    this->RegisterCollection("WFD5ChannelHeaderCollection",WFD5ChannelHeaderPtrCol_);
    this->RegisterCollection("WFD5WaveformHeaderCollection",WFD5WaveformHeaderPtrCol_);
    this->RegisterCollection("WFD5WaveformCollection",WFD5WaveformPtrCol_);

}

WFD5PayloadUnpacker::~WFD5PayloadUnpacker() {};

unpackingStatus WFD5PayloadUnpacker::Unpack(const uint64_t* words, unsigned int& wordNum) {
    LoggerHolder::getInstance().InfoLogger << "\tWe are unpacking a WFD5 payload." << std::endl;

    //Store the starting word for comparison at the end
    unsigned int startWordNum = wordNum;

    /*
        First up is the AMC13+WFD5 header, which is 3 words long
    */

    auto amc_header_words = unpackers::GetXWords(words,wordNum,3,"bigendian");

    //Set the words in the parser
    WFD5HeaderParser_->SetWords(amc_header_words);
    LoggerHolder::getInstance().DebugLogger << WFD5HeaderParser_->Stream().str();

    //Get the AMC slot num
    int amcSlot = WFD5HeaderParser_->AMCNo();

    //Get the data length
    int data_length = WFD5HeaderParser_->DataLength();

    //Get the number of enabled channels
    unsigned int enabled_channels = WFD5HeaderParser_->NumEnabledChannels();

    //Get the time stamp
    clockCounter_ = WFD5HeaderParser_->ClockCounter();

    //Is it synchronous or asynchronous?
    auto aSyncMode = WFD5HeaderParser_->ASyncMode();

    //Is the event empty?
    if (WFD5HeaderParser_->EmptyEvent()) {
        LoggerHolder::getInstance().InfoLogger << "\t\tThis event is empty. Not unpacking." << std::endl;
        //increment the current word and return
        wordNum+=data_length;
        return unpackingStatus::Failure;
    }

    //Create the data product
    WFD5HeaderPtrCol_->push_back(WFD5HeaderParser_->NewDataProduct(crateNum_));

    //Clear data from parser
    WFD5HeaderParser_->Clear();

    /*
        Now we loop over the channels
    */
    for (unsigned int iChannel = 0; iChannel < enabled_channels; ++iChannel)
    {
        //std::cout << "        Proceesing channel: " << iChannel << std::endl;

        //Check which mode we are in: synchronous or asynchronous
        if (aSyncMode) { this->UnpackASyncMode(words,wordNum,amcSlot); }
        else { this->UnpackSyncMode(words,wordNum,amcSlot); }

    }

    //Trailers
    wordNum+=1;

    //Check that we have processed exactly the data_length
    if ((wordNum - startWordNum) != data_length) {
        std::cerr << "Error: didn't parse entire data length\n"
        << "Location: WFD5PayloadUnpacker::Unpack(uint64_t* words, unsigned int& wordNum)\n"
        << "Details: the data length is " << data_length << ", but the number of parse words is " << wordNum - startWordNum << std::endl;
        return unpackingStatus::Failure;
    }

    return unpackingStatus::Success;
};  


void WFD5PayloadUnpacker::UnpackSyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot) {

    /*
        Parse the Channel header, which is 2 64 bit words long
    */
    auto ch_header_words = unpackers::GetXWords(words,wordNum,2,"bigendian");

    //Set the words in the channel parser
    //We won't write the data product yet until we get the trailer
    WFD5ChannelHeaderParser_->SetWords(ch_header_words);
    LoggerHolder::getInstance().DebugLogger << WFD5ChannelHeaderParser_->Stream().str();

    //Get the channel tag
    uint32_t channelTag = WFD5ChannelHeaderParser_->ChannelTag();

    //Get the number of waveforms in this channel
    unsigned int waveform_count = WFD5ChannelHeaderParser_->WaveformCount();

    /*
        Now we loop over each waveform
    */
    for (unsigned int iWaveform = 0; iWaveform < waveform_count; ++iWaveform)
    {
        //std::cout << "            Proceesing waveform: " << iWaveform << std::endl;
        /*
            Parse the Waveform header, which is 2 64 bit words long
        */
        auto wf_header_words = unpackers::GetXWords(words,wordNum,2,"bigendian");

        //Set the words in the parser
        WFD5WaveformHeaderParser_->SetWords(wf_header_words);
        LoggerHolder::getInstance().DebugLogger << WFD5WaveformHeaderParser_->Stream().str();

        //Get the waveform length (number of 8 bit ADC samples = 2 64 bit words)
        uint32_t waveform_length = WFD5WaveformHeaderParser_->WaveformLength();

        uint32_t preTriggerLength = WFD5WaveformHeaderParser_->PreTriggerLength();

        //Get the waveform index
        int waveformIndex = 0;

        //Create the data product
        WFD5WaveformHeaderPtrCol_->push_back(WFD5WaveformHeaderParser_->NewDataProduct(crateNum_,amcSlot));

        //Clean up
        WFD5WaveformHeaderParser_->Clear();

        /*
            Now get the ADC data , which is waveform_length*2 64-bit words
        */
        auto adc_words = unpackers::GetXWords(words,wordNum,waveform_length*2,"le");

        //Set the words in the parser
        WFD5WaveformParser_->SetWords(adc_words);
        LoggerHolder::getInstance().DebugLogger << WFD5WaveformParser_->Stream().str();

        //Create the data product
        WFD5WaveformPtrCol_->push_back(std::make_unique<dataProducts::WFD5Waveform>(
            crateNum_
            ,amcSlot
            ,channelTag
            ,eventNum_
            ,waveformIndex
            ,waveform_length*8 //waveform_length is the number of 8 ADC values
            ,0. // pedestal
            ,0. // pedestal stdev
            ,0  // first sample number
            ,clockCounter_
            ,preTriggerLength*2 //preTriggerLength is in units of 2 ADC values
            ,0  // run number
            ,0  // subrun number
            ,WFD5WaveformParser_->ADCData()
        ));

        //Clean up
        WFD5WaveformParser_->Clear();

    }

    //channel trailer
    auto ch_trailer_words = unpackers::GetXWords(words,wordNum,3,"bigendian");
    WFD5ChannelHeaderParser_->SetTrailer(ch_trailer_words);

    //Create the data product for the channel header now that we have the channel trailer
    WFD5ChannelHeaderPtrCol_->push_back(WFD5ChannelHeaderParser_->NewDataProduct(crateNum_,amcSlot));

    //Clean up the parser
    WFD5ChannelHeaderParser_->Clear();

}

void WFD5PayloadUnpacker::UnpackASyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot) {

    /*
        Parse the Channel header, which is 2 64 bit words long
    */
    auto ch_header_words = unpackers::GetXWords(words,wordNum,2,"bigendian");

    //Set the words in the channel parser
    //We won't write the data product yet until we get the trailer
    WFD5ChannelHeaderAsyncParser_->SetWords(ch_header_words);
    LoggerHolder::getInstance().DebugLogger << WFD5ChannelHeaderAsyncParser_->Stream().str();

    //Get the channel tag
    uint32_t channelTag = WFD5ChannelHeaderAsyncParser_->ChannelTag();

    //Get the number of waveforms in this channel
    unsigned int waveform_count = WFD5ChannelHeaderAsyncParser_->WaveformCount();

    /*
        Now we loop over each waveform
    */
    for (unsigned int iWaveform = 0; iWaveform < waveform_count; ++iWaveform)
    {
        //std::cout << "            Proceesing waveform: " << iWaveform << std::endl;
        /*
            Parse the Waveform header, which is 2 64 bit words long
        */
        auto wf_header_words = unpackers::GetXWords(words,wordNum,2,"bigendian");

        //Set the words in the parser
        WFD5WaveformHeaderAsyncParser_->SetWords(wf_header_words);
        LoggerHolder::getInstance().DebugLogger << WFD5WaveformHeaderAsyncParser_->Stream().str();

        //Get the waveform length (number of 8 bit ADC samples = 2 64 bit words)
        uint32_t waveform_length = WFD5WaveformHeaderAsyncParser_->WaveformLength();

        //Get the waveform index
        int waveformIndex = WFD5WaveformHeaderAsyncParser_->WaveformIndex();

        //In the async mode, each waveform has its own timestamp
        unsigned long thisClockCounter = WFD5WaveformHeaderAsyncParser_->ClockCounter();

        uint32_t preTriggerLength = WFD5WaveformHeaderAsyncParser_->PreTriggerLength();

        //Create the data product
        WFD5WaveformHeaderPtrCol_->push_back(WFD5WaveformHeaderAsyncParser_->NewDataProduct(crateNum_,amcSlot));

        //Clean up
        WFD5WaveformHeaderAsyncParser_->Clear();

        /*
            Now get the ADC data , which is waveform_length*2 64-bit words
        */
        auto adc_words = unpackers::GetXWords(words,wordNum,waveform_length*2,"le");

        //Set the words in the parser
        WFD5WaveformParser_->SetWords(adc_words);
        LoggerHolder::getInstance().DebugLogger << WFD5WaveformParser_->Stream().str();

        //Create the data product
        WFD5WaveformPtrCol_->push_back(std::make_unique<dataProducts::WFD5Waveform>(
            crateNum_
            ,amcSlot
            ,channelTag
            ,eventNum_
            ,waveformIndex
            ,waveform_length*8 //waveform_length is the number of 8 ADC values
            ,0. // pedestal
            ,0. // pedestal stdev
            ,0  // first sample number
            ,thisClockCounter
            ,preTriggerLength*2 //preTriggerLength is in units of 2 ADC values
            ,0  // run number
            ,0  // subrun number
            ,WFD5WaveformParser_->ADCData()
        ));

        //Clean up
        WFD5WaveformParser_->Clear();

    }

    //channel trailer
    auto ch_trailer_words = unpackers::GetXWords(words,wordNum,3,"bigendian");
    WFD5ChannelHeaderAsyncParser_->SetTrailer(ch_trailer_words);

    //Create the data product for the channel header now that we have the channel trailer
    WFD5ChannelHeaderPtrCol_->push_back(WFD5ChannelHeaderAsyncParser_->NewDataProduct(crateNum_,amcSlot));

    //Clean up the parser
    WFD5ChannelHeaderAsyncParser_->Clear();
}

