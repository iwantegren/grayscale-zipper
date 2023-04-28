#include <iostream>
#include <fstream>
#include <bitset>
#include <vector>

#include "ZipperLibrary/Encoder.h"
#include "ZipperLibrary/ImageHandler.h"

int main(int argc, char *argv[])
{
    std::cout << "Welcome to grayscale-zipper" << std::endl;

    RawImageData dummy_data;
    dummy_data.width = 4;
    dummy_data.height = 4;

    unsigned char arr[16] = {
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
        0x00,
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

    // Test compress
    std::vector<Byte> buffer;
    Encoder::compress(dummy_data, buffer);
    for (auto i = 0; i < buffer.size(); ++i)
    {
        if (i > 0 && i % 4 == 0)
            std::cout << std::endl;

        std::bitset<BYTE_SIZE> byte(buffer[i]);
        std::cout << byte << " ";
    }
    std::cout << std::endl;

    std::ofstream fs("example.barch", std::ios::out | std::ios::binary | std::ios::app);
    fs.write(reinterpret_cast<const char *>(buffer.data()), buffer.size() * sizeof(buffer.back()));
    fs.close();

    return 0;
}