#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Common unpackers classes
#pragma link C++ class unpackers::common::Parser+;
#pragma link C++ class unpackers::common::CollectionsHolder+;
#pragma link C++ class unpackers::common::EventUnpacker+;
#pragma link C++ class unpackers::common::BankUnpacker+;
#pragma link C++ class unpackers::common::PayloadUnpacker+;

// WFD5 unpackers classes
#pragma link C++ class unpackers::wfd5::FC7HeaderParser+;
#pragma link C++ class unpackers::wfd5::WaveformParser+;
#pragma link C++ class unpackers::wfd5::WaveformHeaderParser+;
#pragma link C++ class unpackers::wfd5::WaveformHeaderAsyncParser+;
#pragma link C++ class unpackers::wfd5::ChannelHeaderParser+;
#pragma link C++ class unpackers::wfd5::ChannelHeaderAsyncParser+;
#pragma link C++ class unpackers::wfd5::WFD5HeaderParser+;
#pragma link C++ class unpackers::wfd5::CRBankUnpacker+;
#pragma link C++ class unpackers::wfd5::WFD5PayloadUnpacker+;
#pragma link C++ class unpackers::wfd5::FC7PayloadUnpacker+;
#pragma link C++ class unpackers::wfd5::WFD5EventUnpacker+;

// Nalu unpackers classes
#pragma link C++ class unpackers::nalu::ADBankUnpacker+;
#pragma link C++ class unpackers::nalu::ATBankUnpacker+;
#pragma link C++ class unpackers::nalu::ADPayloadUnpacker+;
#pragma link C++ class unpackers::nalu::ATPayloadUnpacker+;
#pragma link C++ class unpackers::nalu::NaluEventUnpacker+;

#endif
