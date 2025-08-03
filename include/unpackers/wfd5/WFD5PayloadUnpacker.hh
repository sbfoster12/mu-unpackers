#ifndef WFD5PAYLOADUNPACKER_HH
#define WFD5PAYLOADUNPACKER_HH

// WFD5 data products
#include "data_products/wfd5/WFD5Header.hh"
#include "data_products/wfd5/WFD5Waveform.hh"

// Common unpackers
#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"

// WFD5 unpackers
#include "unpackers/wfd5/WFD5HeaderParser.hh"
#include "unpackers/wfd5/WFD5ChannelHeaderParser.hh"
#include "unpackers/wfd5/WFD5WaveformHeaderParser.hh"
#include "unpackers/wfd5/WFD5WaveformParser.hh"
#include "unpackers/wfd5/WFD5ChannelHeaderAsyncParser.hh"
#include "unpackers/wfd5/WFD5WaveformHeaderAsyncParser.hh"

namespace unpackers {

    class WFD5PayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        WFD5PayloadUnpacker();

        //Destructor
        ~WFD5PayloadUnpacker();

        unpackingStatus Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Methods to unpack channels in sync and async modes
        void UnpackSyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot);
        void UnpackASyncMode(const uint64_t* words, unsigned int& wordNum, int amcSlot);

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> WFD5HeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> WFD5ChannelHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> WFD5WaveformHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> WFD5WaveformPtrCol_;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5PayloadUnpacker,1)

    private:
        const std::string className_ = "WFD5PayloadUnpacker";

        //Parsers
        std::unique_ptr<unpackers::WFD5HeaderParser> WFD5HeaderParser_;
        std::unique_ptr<unpackers::WFD5ChannelHeaderParser> WFD5ChannelHeaderParser_;
        std::unique_ptr<unpackers::WFD5WaveformHeaderParser> WFD5WaveformHeaderParser_;
        std::unique_ptr<unpackers::WFD5WaveformParser> WFD5WaveformParser_;
        //Async mode
        std::unique_ptr<unpackers::WFD5ChannelHeaderAsyncParser> WFD5ChannelHeaderAsyncParser_;
        std::unique_ptr<unpackers::WFD5WaveformHeaderAsyncParser> WFD5WaveformHeaderAsyncParser_;

        unsigned long clockCounter_; //40 MHz TTC trigger time stamp

    };
}

#endif // WFD5PAYLOADUNPACKER_HH