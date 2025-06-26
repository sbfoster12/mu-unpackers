#include "unpackers/nalu/PacketParser.hh"

using namespace unpackers::nalu;

PacketParser::PacketParser() : unpackers::common::Parser() {};

PacketParser::~PacketParser() {};

// Method to create the data product
data_products::nalu::Packet
PacketParser::NewDataProduct(uint64_t channel_num, uint64_t window_position) {
    return data_products::nalu::Packet(
                channel_num,
                window_position,
                GetTrace()
            );
}

//Get methods
std::vector<short> PacketParser::GetTrace() const { 

    // Initialize the trace
    std::vector<short> trace = {};

    // Loop over each 64-bit word. Careful: the first and last word aren't exclusively samples
    // Each sample is a 2-byte word, to be interpreted as a short
    for (int i_word = 0; i_word < words_.size(); ++i_word) {
        // Loop over the 4 2-byte words
        for (int i = 0; i < 4; ++i) {
            short word = (words_.at(i_word) >> (16 * (3 - i))) & 0xFFFF;
            // std::cout << "word[" << i << "]: 0x"
            //           << std::hex << std::setw(4) << std::setfill('0')
            //           << word << std::endl;

            // Skip first two words in first 64-bit word and last two words in the last 64-bit word  
            if ( ((i_word == 0) && (i == 0 || i == 1)) || ((i_word == 8) && (i == 2 || i == 3)) ) {
                continue;
            }

            //Add the sample!
            trace.push_back(word);
        }
    }

    return trace;
}

std::ostringstream PacketParser::Stream() {
    std::ostringstream oss;
    oss << "    ---> Entering Packet: "<< std::endl;
    oss << "            Trace size: " << GetTrace().size() << std::endl;
    return oss;
}

void PacketParser::Print() {
    std::cout << this->Stream().str();

}
