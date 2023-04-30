#pragma once

#include <vector>

#include "Misc.h"

namespace Decoder
{
    RawImageData decode(std::vector<Byte> &byte_buffer);

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
