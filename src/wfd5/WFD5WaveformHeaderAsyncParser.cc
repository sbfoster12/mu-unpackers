#include "unpackers/wfd5/WFD5WaveformHeaderAsyncParser.hh"

using namespace unpackers;

WFD5WaveformHeaderAsyncParser::WFD5WaveformHeaderAsyncParser() : Parser() {};

WFD5WaveformHeaderAsyncParser::~WFD5WaveformHeaderAsyncParser() {};

std::unique_ptr<dataProducts::WFD5WaveformHeader>
WFD5WaveformHeaderAsyncParser::NewDataProduct(unsigned int crateNum
                                    ,unsigned int amcNum) {
    return std::make_unique<dataProducts::WFD5WaveformHeader>(
                crateNum
                ,amcNum
                ,0
                ,0
                ,0
                ,WaveformIndex()
                ,0
                ,WaveformLength()
                ,DDR3Address()
                ,PreTriggerLength()
                ,XADCAlarm()
                ,D()
                ,TS()
                ,ClockCounter()
            );
}

//Get methods
uint32_t WFD5WaveformHeaderAsyncParser::WaveformIndex1()        const { return (GetWord(WFAsync_WaveformIndex1_WORD) >> WFAsync_WaveformIndex1_BIT) & WFAsync_WaveformIndex1_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::WaveformIndex2()        const { return (GetWord(WFAsync_WaveformIndex2_WORD) >> WFAsync_WaveformIndex2_BIT) & WFAsync_WaveformIndex2_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::WaveformIndex()         const { return (WaveformIndex2() << 12) | WaveformIndex1(); }
uint32_t WFD5WaveformHeaderAsyncParser::DDR3Address()           const { return (GetWord(WFAsync_DDR3Address_WORD) >> WFAsync_DDR3Address_BIT) & WFAsync_DDR3Address_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::PreTriggerLength()      const { return (GetWord(WFAsync_PreTriggerLength_WORD) >> WFAsync_PreTriggerLength_BIT) & WFAsync_PreTriggerLength_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::WaveformLength()        const { return (GetWord(WFAsync_WaveformLength_WORD) >> WFAsync_WaveformLength_BIT) & WFAsync_WaveformLength_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::XADCAlarm()                  const { return (GetWord(WFAsync_XADC_WORD) >> WFAsync_XADC_BIT) & WFAsync_XADC_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::D()                     const { return (GetWord(WFAsync_D_WORD) >> WFAsync_D_BIT) & WFAsync_D_MASK; }
uint32_t WFD5WaveformHeaderAsyncParser::TS()                    const { return (GetWord(WFAsync_TS_WORD) >> WFAsync_TS_BIT) & WFAsync_TS_MASK; }
uint64_t WFD5WaveformHeaderAsyncParser::ClockCounter()             const { return (GetWord(WFAsync_Timestamp_WORD) >> WFAsync_Timestamp_BIT) & WFAsync_Timestamp_MASK; }

std::ostringstream WFD5WaveformHeaderAsyncParser::Stream() {
    std::ostringstream oss;
    oss << "\t\t\t---> WaveformIndex: " << WaveformIndex() << std::endl;
    oss << "\t\t\tPreTriggerLength: " << PreTriggerLength() << std::endl;
    oss << "\t\t\tWaveformLength: " << WaveformLength() << std::endl;
    oss << "\t\t\tXADCAlarms are zero: " << (!XADCAlarm() ? "true" : "false") << std::endl;
    oss << "\t\t\tD: " << D() << std::endl;
    oss << "\t\t\tTS: " << TS() << std::endl;
    oss << "\t\t\tClockCounter: " << ClockCounter() << std::endl;
    oss << "\t\t\tDDR3Address: 0x" << std::hex << DDR3Address() << std::dec << std::endl;
    return oss;
}

void WFD5WaveformHeaderAsyncParser::Print() {
    std::cout << this->Stream().str();

}