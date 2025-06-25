#ifdef __ROOTCLING__

#pragma link off all globals;
#pragma link off all classes;
#pragma link off all functions;

#pragma link C++ namespace unpackers+;
#pragma link C++ class unpackers::CollectionsHolder+;
#pragma link C++ class unpackers::EventUnpacker+;
#pragma link C++ class unpackers::BankUnpacker+;
#pragma link C++ class unpackers::PayloadUnpacker+;
#pragma link C++ class unpackers::ADBankUnpacker+;
#pragma link C++ class unpackers::ATBankUnpacker+;
#pragma link C++ class unpackers::NaluEventUnpacker+;

#endif
