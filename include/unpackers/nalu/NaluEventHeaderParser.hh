#ifndef NALU_EVENTHEADERPARSER_HH
#define NALU_EVENTHEADERPARSER_HH

//Custom
#include "unpackers/common/Parser.hh"
#include <data_products/nalu/NaluEventHeader.hh>

namespace unpackers {

    class NaluEventHeaderParser : public Parser {
        
    public:
        
        //Constructor
        NaluEventHeaderParser();

        //Destructor
        ~NaluEventHeaderParser();

        //Method to create shared ptr
        std::unique_ptr<dataProducts::NaluEventHeader> NewDataProduct();

        //Get methods
        uint16_t    GetEventHeader() const;
        uint16_t    GetEventInfo() const;
        uint32_t    GetEventIndex() const;
        uint32_t    GetEventReferenceTime() const;
        uint16_t    GetPacketSize() const;
        uint64_t    GetChannelMask() const;
        uint16_t    GetNumWindows() const;
        uint16_t    GetNumPackets() const;


        //Print methods
        std::ostringstream Stream() override;
        void Print() override;

    private:
        const std::string className_ = "NaluEventHeaderParser";

        unpackers::DataLocation event_header_data_location_;
        unpackers::DataLocation event_info_data_location_;
        unpackers::DataLocation event_index_data_location_;
        unpackers::DataLocation event_reference_time_data_location_;
        unpackers::DataLocation packet_size_data_location_;
        unpackers::DataLocation channel_mask_data_location_;
        unpackers::DataLocation num_windows_data_location_;
        unpackers::DataLocation num_packets_data_location_;

    };
}

#endif // NALU_EVENTHEADERPARSER_HH