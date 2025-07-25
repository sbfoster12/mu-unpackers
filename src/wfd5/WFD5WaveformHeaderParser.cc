#include "unpackers/wfd5/WFD5WaveformHeaderParser.hh"

using namespace unpackers;

WFD5WaveformHeaderParser::WFD5WaveformHeaderParser() : Parser() {};

WFD5WaveformHeaderParser::~WFD5WaveformHeaderParser() {};

std::unique_ptr<dataProducts::WFD5WaveformHeader>
WFD5WaveformHeaderParser::NewDataProduct(unsigned int crateNum
                                    ,unsigned int amcNum) {
    return std::make_unique<dataProducts::WFD5WaveformHeader>(
                crateNum
                ,amcNum
                ,WaveformCount()
                ,0
                ,ChannelTag()
                ,0
                ,0
                ,WaveformLength()
                ,DDR3Address()
                ,PreTriggerLength()
                ,XADCAlarm()
                ,0
                ,0
                ,0
            );
}

//Get methods
uint32_t WFD5WaveformHeaderParser::WaveformCount()          const  { return (GetWord(WFSync_WaveformCount_WORD) >> WFSync_WaveformCount_BIT) & WFSync_WaveformCount_MASK; }
uint32_t WFD5WaveformHeaderParser::DDR3Address()            const  { return (GetWord(WFSync_DDR3Address_WORD) >> WFSync_DDR3Address_BIT) & WFSync_DDR3Address_MASK; }
uint32_t WFD5WaveformHeaderParser::PreTriggerLength1()      const  { return (GetWord(WFSync_PreTrigLength1_WORD) >> WFSync_PreTrigLength1_BIT) & WFSync_PreTrigLength1_MASK; }
uint32_t WFD5WaveformHeaderParser::PreTriggerLength2()      const  { return (GetWord(WFSync_PreTrigLength2_WORD) >> WFSync_PreTrigLength2_BIT) & WFSync_PreTrigLength2_MASK; }
uint32_t WFD5WaveformHeaderParser::PreTriggerLength()       const  { return (PreTriggerLength2() << 12) | PreTriggerLength1(); }
uint32_t WFD5WaveformHeaderParser::WaveformLength()         const  { return (GetWord(WFSync_WaveformLength_WORD) >> WFSync_WaveformLength_BIT) & WFSync_WaveformLength_MASK; }
uint32_t WFD5WaveformHeaderParser::XADCAlarm()              const  { return (GetWord(WFSync_XADC_WORD) >> WFSync_XADC_BIT) & WFSync_XADC_MASK; }
uint32_t WFD5WaveformHeaderParser::ChannelTag()             const  { return (GetWord(WFSync_ChannelTag_WORD) >> WFSync_ChannelTag_BIT) & WFSync_ChannelTag_MASK; }

std::ostringstream WFD5WaveformHeaderParser::Stream() {
    std::ostringstream oss;
    oss << "\t\t\tWaveformCount: " << WaveformCount() << std::endl;
    oss << "\t\t\tDDR3Address: 0x" << std::hex << DDR3Address() << std::dec << std::endl;
    oss << "\t\t\tPreTriggerLength: " << PreTriggerLength() << std::endl;
    oss << "\t\t\tWaveformLength: " << WaveformLength() << std::endl;
    oss << "\t\t\tXADCAlarm: " << XADCAlarm() << std::endl;
    oss << "\t\t\tChannelTag: " << ChannelTag() << std::endl;
    return oss;
}

void WFD5WaveformHeaderParser::Print() {
    std::cout << this->Stream().str();

}