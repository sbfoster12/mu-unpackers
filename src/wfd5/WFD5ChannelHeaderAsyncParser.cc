#include "unpackers/wfd5/WFD5ChannelHeaderAsyncParser.hh"

using namespace unpackers;

WFD5ChannelHeaderAsyncParser::WFD5ChannelHeaderAsyncParser() : Parser() {};

WFD5ChannelHeaderAsyncParser::~WFD5ChannelHeaderAsyncParser() {};

std::unique_ptr<dataProducts::WFD5ChannelHeader>
WFD5ChannelHeaderAsyncParser::NewDataProduct(unsigned int crateNum
                                    ,unsigned int amcNum) {
    return std::make_unique<dataProducts::WFD5ChannelHeader>(
                crateNum
                ,amcNum
                ,WaveformCount()
                ,0
                ,ChannelTag()
                ,TrigNum()
                ,FillType()
                ,WaveformLength()
                ,0
                ,!DataIntCheck()
                ,PreTriggerLength()
                ,EventLength()
            );
}

uint32_t WFD5ChannelHeaderAsyncParser::WaveformCount() const     { return (GetWord(ChAsync_WaveformCount_WORD)   >> ChAsync_WaveformCount_BIT)   & ChAsync_WaveformCount_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::ChannelTag() const        { return (GetWord(ChAsync_ChannelTag_WORD)      >> ChAsync_ChannelTag_BIT)      & ChAsync_ChannelTag_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::PreTriggerLength() const  { return (GetWord(ChAsync_PreTrigLength_WORD)   >> ChAsync_PreTrigLength_BIT)   & ChAsync_PreTrigLength_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::WaveformLength() const    { return (GetWord(ChAsync_WaveformLength_WORD)  >> ChAsync_WaveformLength_BIT)  & ChAsync_WaveformLength_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::EventLength() const       { return (GetWord(ChAsync_EventLength_WORD)     >> ChAsync_EventLength_BIT)     & ChAsync_EventLength_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::TrigNum() const           { return (GetWord(ChAsync_TrigNum_WORD)         >> ChAsync_TrigNum_BIT)         & ChAsync_TrigNum_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::FillType() const          { return (GetWord(ChAsync_FillType_WORD)        >> ChAsync_FillType_BIT)        & ChAsync_FillType_MASK; }
uint32_t WFD5ChannelHeaderAsyncParser::DataIntCheck() const      { return (GetTrailerWord(ChAsync_DataIntCheck_WORD)    >> ChAsync_DataIntCheck_BIT)    & ChAsync_DataIntCheck_MASK; }

std::ostringstream WFD5ChannelHeaderAsyncParser::Stream() {
    std::ostringstream oss;
    oss << "\t\t---> Entering Channel #" << ChannelTag() << std::endl;
    oss << "\t\tFillType: " << FillType() << std::endl;
    oss << "\t\tWaveformCount: " << WaveformCount() << std::endl;
    oss << "\t\tPreTriggerLength: " << PreTriggerLength() << std::endl;
    oss << "\t\tWaveformLength: " << WaveformLength() << std::endl;
    oss << "\t\tEventLength: " << EventLength() << std::endl;
    oss << "\t\tTrigNum: " << TrigNum() << std::endl;
    return oss;
}

void WFD5ChannelHeaderAsyncParser::Print() {
    std::cout << this->Stream().str();
}