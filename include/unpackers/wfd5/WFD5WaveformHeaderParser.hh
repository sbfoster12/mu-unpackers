#ifndef WFD5_WAVEFORMHEADERPARSER_HH
#define WFD5_WAVEFORMHEADERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include "data_products/wfd5/WFD5WaveformHeader.hh"

// Bit locations and masks for waveform header format:
#define WFSync_WaveformCount_WORD 0
#define WFSync_WaveformCount_BIT 52
#define WFSync_WaveformCount_MASK 0xfff
#define WFSync_DDR3Address_WORD 0
#define WFSync_DDR3Address_BIT 26
#define WFSync_DDR3Address_MASK 0x3ffffff
#define WFSync_PreTrigLength1_WORD 0
#define WFSync_PreTrigLength1_BIT 14
#define WFSync_PreTrigLength1_MASK 0xfff
#define WFSync_PreTrigLength2_WORD 1
#define WFSync_PreTrigLength2_BIT 13
#define WFSync_PreTrigLength2_MASK 0xf
#define WFSync_WaveformLength_WORD 0
#define WFSync_WaveformLength_BIT 0
#define WFSync_WaveformLength_MASK 0x3fff
#define WFSync_XADC_WORD 1
#define WFSync_XADC_BIT 46
#define WFSync_XADC_MASK 0xf
#define WFSync_ChannelTag_WORD 1
#define WFSync_ChannelTag_BIT 34
#define WFSync_ChannelTag_MASK 0xfff

namespace unpackers {

    class WFD5WaveformHeaderParser : public Parser {
        
    public:
        
        //Constructor
        WFD5WaveformHeaderParser();

        //Destructor
        ~WFD5WaveformHeaderParser();

        //Method to make the data product
        std::unique_ptr<dataProducts::WFD5WaveformHeader>
        NewDataProduct(unsigned int crateNum
                    ,unsigned int amcNum); 

        //Get methods
        uint32_t WaveformCount() const;
        uint32_t DDR3Address() const;
        uint32_t PreTriggerLength1() const;
        uint32_t PreTriggerLength2() const;
        uint32_t PreTriggerLength() const;
        uint32_t WaveformLength() const;
        uint32_t XADCAlarm() const;
        uint32_t ChannelTag() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5WaveformHeaderParser,1)

    private:
        const std::string className_ = "WFD5WaveformHeaderParser";

    };
}

#endif // WFD5_WAVEFORMHEADERPARSER_HH