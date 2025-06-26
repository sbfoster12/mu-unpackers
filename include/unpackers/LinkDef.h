#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

// Common unpackers classes
#pragma link C++ namespace unpackers+;
#pragma link C++ class unpackers::common::CollectionsHolder+;
#pragma link C++ class unpackers::common::EventUnpacker+;
#pragma link C++ class unpackers::common::BankUnpacker+;
#pragma link C++ class unpackers::common::PayloadUnpacker+;

// // WFD5 unpackers classes
// #pragma link C++ class unpackers::CRBankUnpacker+;
// #pragma link C++ class unpackers::WFD5Unpacker+;
// #pragma link C++ class unpackers::FC7Unpacker+;
// #pragma link C++ class unpackers::WFD5EventUnpacker+;

// Nalu unpackers classes
#pragma link C++ class unpackers::nalu::ADBankUnpacker+;
#pragma link C++ class unpackers::nalu::ATBankUnpacker+;
#pragma link C++ class unpackers::nalu::NaluEventUnpacker+;

#endif
