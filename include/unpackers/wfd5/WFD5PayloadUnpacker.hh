#ifndef WFD5PAYLOADUNPACKER_HH
#define WFD5PAYLOADUNPACKER_HH

// WFD5 data products
#include "data_products/wfd5/WFD5Header.hh"
#include "data_products/wfd5/Waveform.hh"

// Common unpackers
#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"

// WFD5 unpackers
#include "unpackers/wfd5/WFD5HeaderParser.hh"
#include "unpackers/wfd5/ChannelHeaderParser.hh"
#include "unpackers/wfd5/WaveformHeaderParser.hh"
#include "unpackers/wfd5/WaveformParser.hh"
#include "unpackers/wfd5/ChannelHeaderAsyncParser.hh"
#include "unpackers/wfd5/WaveformHeaderAsyncParser.hh"

namespace unpackers::wfd5 {

    class WFD5PayloadUnpacker : public unpackers::common::PayloadUnpacker {
        
    public:
        
        //Constructor
        WFD5PayloadUnpacker();

        //Destructor
        ~WFD5PayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Methods to unpack channels in sync and async modes
        void UnpackSyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot);
        void UnpackASyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot);

        //Collections
        std::shared_ptr<data_products::common::DataProductPtrCollection> WFD5HeaderPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> ChannelHeaderPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> WaveformHeaderPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> WaveformPtrCol_;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5PayloadUnpacker,1)

    private:
        const std::string className_ = "WFD5PayloadUnpacker";

        //Parsers
        std::unique_ptr<unpackers::wfd5::WFD5HeaderParser> WFD5HeaderParser_;
        std::unique_ptr<unpackers::wfd5::ChannelHeaderParser> ChannelHeaderParser_;
        std::unique_ptr<unpackers::wfd5::WaveformHeaderParser> WaveformHeaderParser_;
        std::unique_ptr<unpackers::wfd5::WaveformParser> WaveformParser_;
        //Async mode
        std::unique_ptr<unpackers::wfd5::ChannelHeaderAsyncParser> ChannelHeaderAsyncParser_;
        std::unique_ptr<unpackers::wfd5::WaveformHeaderAsyncParser> WaveformHeaderAsyncParser_;

        unsigned long clockCounter_; //40 MHz TTC trigger time stamp

    };
}

#endif // WFD5PAYLOADUNPACKER_HH