#ifndef WAVEFORMPARSER_HH
#define WAVEFORMPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include "data_products/wfd5/Waveform.hh"

// Bit locations and masks for WFD5 waveform format:

namespace unpackers::wfd5 {

    class WaveformParser : public unpackers::common::Parser {

    public:
        
        //Constructor
        WaveformParser();

        //Destructor
        ~WaveformParser();

        //Get methods
        std::vector<short>  ADCData() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WaveformParser,1)

    private:
        std::string className_ = "WaveformParser";

    };
}

#endif // WAVEFORMPARSER_HH