#pragma once

#include <vector>
#include <string>

#include "Misc.h"

namespace Decoder
{
    RawImageData decode(std::vector<Byte> &byte_buffer);

    void readEncoded(const std::string &filename, std::vector<Byte> &out_buffer);
    void writeDecoded(const std::string &filename, const RawImageData &data);

    class Bitstream
    {
    public:
        Bitstream(const std::vector<Byte> &buffer);

        bool popBit();
        Byte popByte();
        Word popWord();

    private:
        const std::vector<Byte> &raw_buffer;

        int byte_idx;
        int bit_pos;
    };
}
