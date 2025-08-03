#ifndef NALU_ADPAYLOADUNPACKER_HH
#define NALU_ADPAYLOADUNPACKER_HH

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

    class NaluADPayloadUnpacker : public PayloadUnpacker {
        
    public:
        
        //Constructor
        NaluADPayloadUnpacker();

        //Destructor
        ~NaluADPayloadUnpacker();

        unpackingStatus Unpack(const uint64_t* words, unsigned int& wordNum) override;

        //Collections
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluEventHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluPacketHeaderPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluPacketFooterPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluWaveformPtrCol_;
        std::shared_ptr<dataProducts::DataProductPtrCollection> naluEventFooterPtrCol_;

        /// ROOT class definition macro with versioning
        ClassDefOverride(NaluADPayloadUnpacker,1)

    private:
        const std::string className_ = "NaluADPayloadUnpacker";

        //Parsers
        std::unique_ptr<NaluEventHeaderParser> naluEventHeaderParser_;
        std::unique_ptr<NaluPacketHeaderParser> naluPacketHeaderParser_;
        std::unique_ptr<NaluPacketParser> naluPacketParser_;
        std::unique_ptr<NaluPacketFooterParser> naluPacketFooterParser_;
        std::unique_ptr<NaluEventFooterParser> naluEventFooterParser_;

    };
}

#endif // NALU_ADPAYLOADUNPACKER_HH