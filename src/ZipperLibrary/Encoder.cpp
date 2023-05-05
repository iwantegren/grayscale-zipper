#include "include/Encoder.h"

#include "include/ImageHandler.h"
#include <fstream>
#include "include/ZipperException.h"

namespace Encoder
{
    void encode(RawImageData &raw_data, std::vector<Byte> &out_buffer)
    {
        ImageHandler image(raw_data);
        const int height = raw_data.height;
        const int width = raw_data.width;

        Bitstream bitstream;

        // Add width & height
        bitstream.push(static_cast<Word>(width));
        bitstream.push(static_cast<Word>(height));

        // Create and add array of empty indices
        std::vector<bool> empty_rows(height);
        for (auto row = 0; row < height; ++row)
        {
            bool is_empty = image.isEmptyRow(row);

            empty_rows[row] = is_empty;
            bitstream.push(is_empty);
        }

        // Add non-empty rows
        for (auto row = 0; row < height; ++row)
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

    RawImageData readDecoded(const std::string &filename)
    {
        RawImageData out_data;

        std::ifstream infile(filename, std::ios::in | std::ios::binary);
        if (!infile.is_open())
            throw ZipperException("Can't open file for reading!");

        infile.seekg(0, std::ios::end);
        size_t file_size = infile.tellg();
        infile.seekg(0, std::ios::beg);

        if (file_size < sizeof(RawImageData))
        {
            throw ZipperException("File is empty!");
        }

        infile.read(reinterpret_cast<char *>(&out_data.width), sizeof(out_data.width));
        infile.read(reinterpret_cast<char *>(&out_data.height), sizeof(out_data.height));
        auto length = out_data.width * out_data.height;

        if (file_size != length + sizeof(out_data.width) + sizeof(out_data.height))
        {
            throw ZipperException("Wrong file content!");
        }

        out_data.data = new unsigned char[length]();
        infile.read(reinterpret_cast<char *>(out_data.data), sizeof(Byte) * length);
        infile.close();

        return out_data;
    }

    void writeEncoded(const std::string &filename, std::vector<Byte> &data)
    {
        std::ofstream outfile(filename, std::ios::out | std::ios::binary);
        if (!outfile.is_open())
            throw ZipperException("Can't open file for writing!");

        if (data.empty())
        {
            throw ZipperException("No data for writing!");
        }

        outfile.write(reinterpret_cast<const char *>(data.data()), sizeof(Byte) * data.size());
        outfile.close();
    }

    void Bitstream::push(bool bit)
    {
        raw_bits.push_back(bit);
    }

    void Bitstream::push(Byte byte)
    {
        Byte mask = Byte(1 << 7);
        for (auto i = 0; i < BYTE_SIZE; ++i)
        {
            raw_bits.push_back(mask & byte);
            mask >>= 1;
        }
    }

    void Bitstream::push(Word data)
    {
        Word mask = 1 << 15;
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
