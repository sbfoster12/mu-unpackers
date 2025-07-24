#ifndef WFD5_WAVEFORMPARSER_HH
#define WFD5_WAVEFORMPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include "data_products/wfd5/WFD5Waveform.hh"

// Bit locations and masks for WFD5 waveform format:

namespace unpackers {

    class WFD5WaveformParser : public Parser {

    public:
        
        //Constructor
        WFD5WaveformParser();

        //Destructor
        ~WFD5WaveformParser();

        //Get methods
        std::vector<short>  ADCData() const;

        //Print method
        std::ostringstream Stream() override;
        void Print() override;

        // ROOT class definition macro with versioning
        ClassDefOverride(WFD5WaveformParser,1)

    private:
        std::string className_ = "WFD5WaveformParser";

    };
}

#endif // WFD5_WAVEFORMPARSER_HH