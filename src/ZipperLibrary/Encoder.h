#pragma once

#include <vector>

#include "Misc.h"

namespace Encoder
{
    void compress(RawImageData &raw_data, std::vector<Byte> &out_buffer);

    class Bitstream
    {
    public:
        Bitstream(){};

        void push(bool bit);
        void push(Byte byte);
        void push(unsigned short data);
        void popBytes(std::vector<Byte> &bytes);

    private:
        std::vector<bool> raw_bits;
    };
}
