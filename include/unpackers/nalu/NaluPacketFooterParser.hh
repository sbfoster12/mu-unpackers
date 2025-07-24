#ifndef NALU_PACKETFOOTERPARSER_HH
#define NALU_PACKETFOOTERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/NaluPacketFooter.hh>

namespace unpackers {

    class NaluPacketFooterParser : public Parser {
        
    public:
        
        //Constructor
        NaluPacketFooterParser();

        //Destructor
        ~NaluPacketFooterParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluPacketFooter> NewDataProduct();

        //Get methods
        uint32_t        GetParserIndex() const;
        uint32_t        GetPacketFooter() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluPacketFooterParser";

        unpackers::DataLocation parser_index_data_location_;
        unpackers::DataLocation packet_footer_data_location_;

    };
}

#endif // NALU_PACKETFOOTERPARSER_HH