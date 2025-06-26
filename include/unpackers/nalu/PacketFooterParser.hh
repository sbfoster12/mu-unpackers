#ifndef NALU_PACKETFOOTERERPARSER_HH
#define NALU_PACKETFOOTERERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/PacketFooter.hh>

namespace unpackers::nalu {

    class PacketFooterParser : public unpackers::common::Parser {
        
    public:
        
        //Constructor
        PacketFooterParser();

        //Destructor
        ~PacketFooterParser();

        //Method to create shared ptr
        std::unique_ptr<data_products::nalu::PacketFooter> NewDataProduct();

        //Get methods
        uint32_t        GetParserIndex() const;
        uint32_t        GetPacketFooter() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "PacketFooterParser";

        unpackers::common::DataLocation parser_index_data_location_;
        unpackers::common::DataLocation packet_footer_data_location_;

    };
}

#endif // NALU_PACKETFOOTERERPARSER_HH