#include "include/Decoder.h"

#include <algorithm>
#include "include/ImageHandler.h"
#include "include/Encoder.h"
#include <fstream>
#include <iostream>
#include "include/ZipperException.h"
#include <iterator>

namespace Decoder
{
    RawImageData decode(std::vector<Byte> &byte_buffer)
    {
        using Encoder::BLOCK_SIZE;

        RawImageData out_raw_data;

        Bitstream bitstream(byte_buffer);

        try
        {
            // Read width & height
            auto width = bitstream.popWord();
            auto height = bitstream.popWord();

            out_raw_data.width = width;
            out_raw_data.height = height;

            // Read array of empty indices
            std::vector<bool> empty_rows(height);
            for (auto row = 0; row < height; ++row)
            {
                empty_rows[row] = bitstream.popBit();
            }

            // Init data
            out_raw_data.data = new unsigned char[height * width]();
            ImageHandler image(out_raw_data);

            for (auto row = 0; row < height; ++row)
            {
                if (empty_rows[row])
                {
                    image.fillSequence(row, 0, width, ImageHandler::WHITE);
                }
                else
                {
                    auto col = 0;
                    for (; col + BLOCK_SIZE <= width; col += BLOCK_SIZE)
                    {
                        if (bitstream.popBit() == 0) // 0b0 for 4 white pixels
                        {
                            image.fillSequence(row, col, BLOCK_SIZE, ImageHandler::WHITE);
                        }
                        else if (bitstream.popBit() == 0) // 0b10 for 4 black pixles
                        {
                            image.fillSequence(row, col, BLOCK_SIZE, ImageHandler::BLACK);
                        }
                        else // 0b11 for 4 other pixles
                        {
                            // read 4 bytes for colors
                            for (auto i = 0; i < BLOCK_SIZE; ++i)
                            {
                                image.set(row, col, bitstream.popByte());
                            }
                        }
                    }

                    // Handle case where width not aligned into 4 pixels
                    if (col < width)
                    {
                        // Read 0b11
                        bitstream.popBit();
                        bitstream.popBit();

                        const auto pixels_left = width - col;

                        auto i = 0;
                        for (; i < pixels_left; ++i)
                        {
                            image.set(row, col, bitstream.popByte());
                        }
                        for (; i < BLOCK_SIZE; ++i)
                        {
                            bitstream.popByte(); // pop excessive bytes
                        }
                    }
                }
            }
        }
        catch (ZipperException e)
        {
            throw ZipperException("Wrong file content!");
        }

        return out_raw_data;
    }

    void readEncoded(const std::string &filename, std::vector<Byte> &out_buffer)
    {
        std::ifstream infile(filename, std::ios::in | std::ios::binary);
        if (!infile.is_open())
            throw ZipperException("Can't open file for reading!");

        infile.seekg(0, std::ios::end);
        size_t file_size = infile.tellg();
        infile.seekg(0, std::ios::beg);
        if (file_size == 0)
        {
            throw ZipperException("File is empty!");
        }

        out_buffer.clear();
        out_buffer.reserve(file_size);
        out_buffer.insert(out_buffer.begin(),
                          std::istream_iterator<Byte>(infile),
                          std::istream_iterator<Byte>());
        infile.close();
    }

    void writeDecoded(const std::string &filename, const RawImageData &data)
    {
        std::ofstream outfile(filename, std::ios::out | std::ios::binary);
        if (!outfile.is_open())
            throw ZipperException("Can't open file for writing!");

        outfile.write(reinterpret_cast<const char *>(&data.width), sizeof(data.width));
        outfile.write(reinterpret_cast<const char *>(&data.height), sizeof(data.height));

        if (data.data == nullptr)
        {
            throw ZipperException("No data for writing!");
        }
        outfile.write(reinterpret_cast<const char *>(data.data), data.width * data.height);
        outfile.close();
    }

    Bitstream::Bitstream(const std::vector<Byte> &buffer)
        : raw_buffer(buffer), byte_idx(0), bit_pos(BYTE_SIZE - 1)
    {
    }

    bool Bitstream::popBit()
    {
        if (byte_idx >= raw_buffer.size())
        {
            throw ZipperException("Out of buffer");
        }

        bool bit = raw_buffer[byte_idx] & (1 << bit_pos);

        --bit_pos;
        if (bit_pos < 0)
        {
            bit_pos = BYTE_SIZE - 1;
            ++byte_idx;
        }

        return bit;
    }

    Byte Bitstream::popByte()
    {
        Byte byte;
        for (auto i = 0; i < BYTE_SIZE; ++i)
        {
            byte <<= 1;
            byte |= popBit();
        }

        return byte;
    }

    Word Bitstream::popWord()
    {
        Word word;
        for (auto i = 0; i < BYTE_SIZE * sizeof(Word); ++i)
        {
            word <<= 1;
            word |= popBit();
        }

        return word;
    }
}
