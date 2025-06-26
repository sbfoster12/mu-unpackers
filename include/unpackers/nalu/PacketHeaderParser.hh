#ifndef NALU_PACKETHEADERPARSER_HH
#define NALU_PACKETHEADERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/PacketHeader.hh>

namespace unpackers::nalu {

    class PacketHeaderParser : public unpackers::common::Parser {
        
    public:
        
        //Constructor
        PacketHeaderParser();

        //Destructor
        ~PacketHeaderParser();

        //Method to create shared ptr
        std::unique_ptr<data_products::nalu::PacketHeader> NewDataProduct();

        //Get methods
        uint16_t          GetPacketHeader() const;
        uint16_t          GetPacketInfo() const;
        uint16_t          GetChannel() const;
        uint32_t          GetTriggerTime() const;
        uint16_t          GetLogicalPosition() const;
        uint16_t          GetWindowPosition() const;

        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "PacketHeaderParser";

        unpackers::common::DataLocation packet_header_data_location_;
        unpackers::common::DataLocation packet_info_data_location_;
        unpackers::common::DataLocation channel_data_location_;
        unpackers::common::DataLocation trigger_time_data_location_;
        unpackers::common::DataLocation logical_position_data_location_;
        unpackers::common::DataLocation window_position_data_location_;

    };
}

#endif // NALU_PACKETHEADERPARSER_HH