#ifndef NALU_EVENTFOOTERPARSER_HH
#define NALU_EVENTFOOTERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/NaluEventFooter.hh>

namespace unpackers {

    class NaluEventFooterParser : public Parser {
        
    public:
        
        //Constructor
        NaluEventFooterParser();

        //Destructor
        ~NaluEventFooterParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluEventFooter> NewDataProduct();

        //Get methods
        uint32_t        GetEventFooter() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluEventFooterParser";

        unpackers::DataLocation event_footer_data_location_;

    };
}

#endif // NALU_EVENTFOOTERPARSER_HH