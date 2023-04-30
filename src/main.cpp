#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>

#include "ZipperLibrary/include/Encoder.h"
#include "ZipperLibrary/include/Decoder.h"
#include "ZipperLibrary/include/ImageHandler.h"

int main(int argc, char *argv[])
{
    std::cout << "Welcome to grayscale-zipper" << std::endl;

    RawImageData dummy_data;
    dummy_data.width = 3;
    dummy_data.height = 6;

    unsigned char arr[18] = {
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,

        0x00,
        0x00,
        0x00,
        0x01,
        0x01,
        0xff,
        0xff,
        0xff,

        0xff,
        0xff,
    };
    dummy_data.data = arr;

    ImageHandler handler(dummy_data);

    for (auto i = 0; i < dummy_data.height; ++i)
    {
        std::cout << "Row[" << i << "] is " << (handler.isEmptyRow(i) ? "empty" : "not empty") << std::endl;
    }

    for (auto i = 0; i < dummy_data.height; ++i)
    {
        for (auto j = 0; j < dummy_data.width; ++j)
            std::cout << static_cast<int>(handler.get(i, j)) << ", ";
        std::cout << std::endl;
    }

    // Test encode
    std::vector<Byte> buffer;
    Encoder::encode(dummy_data, buffer);
    for (auto i = 0; i < buffer.size(); ++i)
    {
        if (i > 0 && i % 4 == 0)
            std::cout << std::endl;

        std::bitset<BYTE_SIZE> byte(buffer[i]);
        std::cout << byte << " ";
    }
    std::cout << std::endl;

    std::ofstream fs("example.barch", std::ios::out | std::ios::binary);
    fs.write(reinterpret_cast<const char *>(buffer.data()), buffer.size() * sizeof(buffer.back()));
    fs.close();

    std::ifstream infile("example.barch");

    infile.seekg(0, std::ios::end);
    size_t length = infile.tellg();
    infile.seekg(0, std::ios::beg);

    std::vector<Byte> raw_bytes;
    for (int i = 0; i < length; ++i)
    {
        char byte;
        infile.read(&byte, sizeof(byte));
        raw_bytes.push_back(byte);
    }

    Decoder::Bitstream decoder_stream(raw_bytes);
    for (auto i = 0; i < raw_bytes.size(); ++i)
    {
        if (i > 0 && i % 4 == 0)
            std::cout << std::endl;

        std::bitset<BYTE_SIZE> byte(decoder_stream.popByte());
        std::cout << byte << " ";
    }
    std::cout << std::endl;

    auto decoded_image = Decoder::decode(raw_bytes);
    for (auto i = 0; i < decoded_image.height; ++i)
    {
        for (auto j = 0; j < decoded_image.width; ++j)
            std::cout << static_cast<int>(handler.get(i, j)) << ", ";
        std::cout << std::endl;
    }

    return 0;
}