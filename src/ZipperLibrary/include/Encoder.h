#pragma once

#include <vector>
#include <string>

#include "Misc.h"

namespace Encoder
{
    void encode(RawImageData &raw_data, std::vector<Byte> &out_buffer);

    RawImageData readDecoded(const std::string &filename);
    void writeEncoded(const std::string &filename, std::vector<Byte> &data);

    class Bitstream
    {
    public:
        Bitstream(){};

        void push(bool bit);
        void push(Byte byte);
        void push(Word data);
        void popBytes(std::vector<Byte> &bytes);

    private:
        std::vector<bool> raw_bits;
    };

    constexpr int BLOCK_SIZE = 4;
}
