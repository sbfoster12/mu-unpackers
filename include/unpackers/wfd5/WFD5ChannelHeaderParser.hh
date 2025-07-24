#ifndef WFD5_CHANNELHEADERPARSER_HH
#define WFD5_CHANNELHEADERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include "data_products/wfd5/WFD5ChannelHeader.hh"

// Bit locations and masks for WFD5 channel format:
#define ChSync_ChannelTag_WORD 0
#define ChSync_ChannelTag_BIT 46
#define ChSync_ChannelTag_MASK 0xfff
#define ChSync_PreTrigLength_WORD 0
#define ChSync_PreTrigLength_BIT 24
#define ChSync_PreTrigLength_MASK 0xffff
#define ChSync_WaveformCount_WORD 0
#define ChSync_WaveformCount_BIT 12
#define ChSync_WaveformCount_MASK 0xfff
#define ChSync_DDR3Address_1_WORD 1
#define ChSync_DDR3Address_1_BIT 50
#define ChSync_DDR3Address_1_MASK 0x3fff
#define ChSync_DDR3Address_2_WORD 0
#define ChSync_DDR3Address_2_BIT 0
#define ChSync_DDR3Address_2_MASK 0xfff
#define ChSync_WaveformLength_WORD 1
#define ChSync_WaveformLength_BIT 27
#define ChSync_WaveformLength_MASK 0x7fffff
#define ChSync_FillType_WORD 1
#define ChSync_FillType_BIT 24
#define ChSync_FillType_MASK 0x7
#define ChSync_TrigNum_WORD 1
#define ChSync_TrigNum_BIT 0
#define ChSync_TrigNum_MASK 0xffffff
#define ChSync_DataIntCheck_WORD 2
#define ChSync_DataIntCheck_BIT 32
#define ChSync_DataIntCheck_MASK 0xffffffff

namespace unpackers {

    class WFD5ChannelHeaderParser : public Parser {

    public:
        
        //Constructor
        WFD5ChannelHeaderParser();

        //Destructor
        ~WFD5ChannelHeaderParser();

        //Method to make the data product
        std::unique_ptr<dataProducts::WFD5ChannelHeader>
        NewDataProduct(unsigned int crateNum
                    ,unsigned int amcNum);

        uint32_t ChannelTag() const;
        uint32_t PreTriggerLength() const;
        uint32_t WaveformCount() const;
        uint32_t DDR3Address_1() const;
        uint32_t DDR3Address_2() const;
        uint32_t DDR3Address() const;
        uint32_t WaveformLength() const;
        uint32_t FillType() const;
        uint32_t TrigNum() const;
        uint32_t DataIntCheck() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5ChannelHeaderParser,1)

    private:
        const std::string className_ = "WFD5ChannelHeaderParser";

    };
}

#endif // WFD5_CHANNELHEADERPARSER_HH