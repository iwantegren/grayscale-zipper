#pragma once

#include <cstddef>

struct RawImageData
{
    int width;           // image width in pixels
    int height;          // image height in pixels
    unsigned char *data; // Pointer to image data. data[j * width + i] is color of pixel in row j and column i.
};

using Byte = unsigned char;
using Word = unsigned short;

constexpr std::size_t BYTE_SIZE = 8;
