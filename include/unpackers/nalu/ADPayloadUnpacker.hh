#ifndef NALU_ADPAYLOADUNPACKER_HH
#define NALU_ADPAYLOADUNPACKER_HH

#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/nalu/EventHeaderParser.hh"
#include "unpackers/nalu/PacketHeaderParser.hh"
#include "unpackers/nalu/PacketParser.hh"
#include "unpackers/nalu/PacketFooterParser.hh"
#include "unpackers/nalu/EventFooterParser.hh"
#include "data_products/nalu/EventHeader.hh"
#include "data_products/nalu/PacketHeader.hh"
#include "data_products/nalu/Packet.hh"
#include "data_products/nalu/Waveform.hh"
#include "data_products/nalu/PacketFooter.hh"
#include "data_products/nalu/EventFooter.hh"

namespace unpackers::nalu {

    class ADPayloadUnpacker : public unpackers::common::PayloadUnpacker {
        
    public:
        
        //Constructor
        ADPayloadUnpacker();

        //Destructor
        ~ADPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<data_products::common::DataProductPtrCollection> eventHeaderPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> packetHeaderPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> packetFooterPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> waveformPtrCol_;
        std::shared_ptr<data_products::common::DataProductPtrCollection> eventFooterPtrCol_;

    private:
        const std::string className_ = "ADPayloadUnpacker";

        //Parsers
        std::unique_ptr<EventHeaderParser> eventHeaderParser_;
        std::unique_ptr<PacketHeaderParser> packetHeaderParser_;
        std::unique_ptr<PacketParser> packetParser_;
        std::unique_ptr<PacketFooterParser> packetFooterParser_;
        std::unique_ptr<EventFooterParser> eventFooterParser_;

    };
}

#endif // NALU_ADPAYLOADUNPACKER_HH