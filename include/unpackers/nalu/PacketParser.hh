#ifndef NALU_PACKETPARSER_HH
#define NALU_PACKETPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/Packet.hh>

namespace unpackers::nalu {

    class PacketParser : public unpackers::common::Parser {
        
    public:
        
        //Constructor
        PacketParser();

        //Destructor
        ~PacketParser();

        //Method to create shared ptr
        data_products::nalu::Packet NewDataProduct(uint64_t channel_num, uint64_t window_position);

        //Get methods
        std::vector<short> GetTrace() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "PacketParser";

    };
}

#endif // NALU_PACKETPARSER_HH