#ifndef ADPAYLOADUNPACKER_HH
#define ADPAYLOADUNPACKER_HH

#include "unpackers/common/Logger.hh"
#include "unpackers/common/PayloadUnpacker.hh"
#include "unpackers/common/UnpackerHelpers.hh"
#include "unpackers/nalu/NaluEventHeaderParser.hh"
#include "unpackers/nalu/NaluPacketHeaderParser.hh"
#include "unpackers/nalu/NaluPacketParser.hh"
#include "unpackers/nalu/NaluPacketFooterParser.hh"
#include "unpackers/nalu/NaluEventFooterParser.hh"
#include "data_products/nalu/NaluEventHeader.hh"
#include "data_products/nalu/NaluPacketHeader.hh"
#include "data_products/nalu/NaluPacket.hh"
#include "data_products/nalu/NaluWaveform.hh"
#include "data_products/nalu/NaluPacketFooter.hh"
#include "data_products/nalu/NaluEventFooter.hh"

namespace unpackers {

    class ADPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        ADPayloadUnpacker();

        //Destructor
        ~ADPayloadUnpacker();

        int Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluEventHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluPacketHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluPacketFooterPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluWaveformPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluEventFooterPtrCol_;

    private:
        const std::string className_ = "ADPayloadUnpacker";

        //Parsers
        std::unique_ptr<parsers::NaluEventHeaderParser> naluEventHeaderParser_;
        std::unique_ptr<parsers::NaluPacketHeaderParser> naluPacketHeaderParser_;
        std::unique_ptr<parsers::NaluPacketParser> naluPacketParser_;
        std::unique_ptr<parsers::NaluPacketFooterParser> naluPacketFooterParser_;
        std::unique_ptr<parsers::NaluEventFooterParser> naluEventFooterParser_;

    };
}

#endif // ADPAYLOADUNPACKER_HH