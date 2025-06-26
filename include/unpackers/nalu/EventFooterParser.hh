#ifndef NALU_EVENTFOOTERPARSER_HH
#define NALU_EVENTFOOTERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/EventFooter.hh>

namespace unpackers::nalu {

    class EventFooterParser : public unpackers::common::Parser {
        
    public:
        
        //Constructor
        EventFooterParser();

        //Destructor
        ~EventFooterParser();

        //Method to create shared ptr
        std::unique_ptr<data_products::nalu::EventFooter> NewDataProduct();

        //Get methods
        uint32_t        GetEventFooter() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "EventFooterParser";

        unpackers::common::DataLocation event_footer_data_location_;

    };
}

#endif // NALU_EVENTFOOTERPARSER_HH