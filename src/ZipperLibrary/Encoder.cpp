#include "Encoder.h"

#include "ImageHandler.h"

namespace Encoder
{
    constexpr int BLOCK_SIZE = 4;

    void compress(RawImageData &raw_data, std::vector<Byte> &out_buffer)
    {
        ImageHandler image(raw_data);
        const int heigth = raw_data.height;
        const int width = raw_data.width;

        Bitstream bitstream;

        // Add width & heigth
        bitstream.push(static_cast<unsigned short>(width));
        bitstream.push(static_cast<unsigned short>(heigth));

        // Create and add array of empty indices
        std::vector<bool> empty_rows(heigth);
        for (auto row = 0; row < heigth; ++row)
        {
            bool is_empty = image.isEmptyRow(row);

            empty_rows[row] = is_empty;
            bitstream.push(is_empty);
        }

        // Add non-empty rows
        for (auto row = 0; row < heigth; ++row)
        {
            // Skip empty row
            if (empty_rows[row] == true)
                continue;

            auto col = 0;
            for (; col + BLOCK_SIZE <= width; col += BLOCK_SIZE)
            {
                if (image.checkSequence(row, col, BLOCK_SIZE, ImageHandler::WHITE))
                {
                    // push 0b0 for white sequence
                    bitstream.push(false);
                }
                else if (image.checkSequence(row, col, BLOCK_SIZE, ImageHandler::BLACK))
                {
                    // push 0b10 black sequence
                    bitstream.push(true);
                    bitstream.push(false);
                }
                else
                {
                    // push 0b11
                    bitstream.push(true);
                    bitstream.push(true);

                    // push 4 bytes for colors
                    for (auto i = 0; i < BLOCK_SIZE; ++i)
                    {
                        bitstream.push(image.get(row, col + i));
                    }
                }
            }

            // Handle case where width not aligned into 4 pixels
            if (col < width)
            {
                // push 0b11
                bitstream.push(true);
                bitstream.push(true);

                const auto pixels_left = width - col;

                auto i = 0;
                for (; i < pixels_left; ++i)
                {
                    bitstream.push(image.get(row, col + i));
                }
                for (; i < BLOCK_SIZE; ++i)
                {
                    bitstream.push(ImageHandler::WHITE);
                }
            }
        }

        bitstream.popBytes(out_buffer);
    }

    void Bitstream::push(bool bit)
    {
        raw_bits.push_back(bit);
    }

    void Bitstream::push(Byte byte)
    {
        Byte mask = Byte(1000'0000);
        for (auto i = 0; i < BYTE_SIZE; ++i)
        {
            raw_bits.push_back(mask & byte);
            mask >>= 1;
        }
    }

    void Bitstream::push(unsigned short data)
    {
        unsigned short mask = 1000'0000'0000'0000;
        for (auto i = 0; i < BYTE_SIZE * sizeof(data); ++i)
        {
            raw_bits.push_back(mask & data);
            mask >>= 1;
        }
    }

    void Bitstream::popBytes(std::vector<Byte> &bytes)
    {
        // Fill array with empty bits to have 8-sized blocks
        if (raw_bits.size() % BYTE_SIZE)
        {
            auto missing_bits = BYTE_SIZE - (raw_bits.size() % BYTE_SIZE);
            for (auto i = 0; i < missing_bits; ++i)
                raw_bits.push_back(false);
        }

        // Pack 8 bits in bytes
        const auto byte_count = raw_bits.size() / BYTE_SIZE;
        for (auto byte_idx = 0; byte_idx < byte_count; ++byte_idx)
        {
            char byte;
            for (auto i = 0; i < BYTE_SIZE; ++i)
            {
                byte <<= 1;
                byte |= raw_bits[i + (byte_idx * BYTE_SIZE)];
            }
            bytes.push_back(byte);
        }
    }
}
