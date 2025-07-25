#ifndef NALU_PACKETPARSER_HH
#define NALU_PACKETPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/NaluPacket.hh>

namespace unpackers {

    class NaluPacketParser : public Parser {
        
    public:
        
        //Constructor
        NaluPacketParser();

        //Destructor
        ~NaluPacketParser();

        //Method to create shared ptr
        dataProducts::NaluPacket NewDataProduct(uint64_t channel_num, uint64_t window_position);

        //Get methods
        std::vector<short> GetTrace() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluPacketParser";

    };
}

#endif // NALU_PACKETPARSER_HH