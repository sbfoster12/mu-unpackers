#ifndef NALU_PACKETHEADERPARSER_HH
#define NALU_PACKETHEADERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/NaluPacketHeader.hh>

namespace unpackers {

    class NaluPacketHeaderParser : public Parser {
        
    public:
        
        //Constructor
        NaluPacketHeaderParser();

        //Destructor
        ~NaluPacketHeaderParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluPacketHeader> NewDataProduct();

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
        const std::string className_ = "NaluPacketHeaderParser";

        unpackers::DataLocation packet_header_data_location_;
        unpackers::DataLocation packet_info_data_location_;
        unpackers::DataLocation channel_data_location_;
        unpackers::DataLocation trigger_time_data_location_;
        unpackers::DataLocation logical_position_data_location_;
        unpackers::DataLocation window_position_data_location_;

    };
}

#endif // NALU_PACKETHEADERPARSER_HH