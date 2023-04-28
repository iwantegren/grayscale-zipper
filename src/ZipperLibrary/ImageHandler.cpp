#include "ImageHandler.h"

#include <algorithm>

ImageHandler::ImageHandler(RawImageData &image)
    : raw_image(image)
{
}

Byte ImageHandler::get(int row, int col) const
{
    return raw_image.data[row * raw_image.width + col];
}

void ImageHandler::set(int row, int col, Byte val)
{
    raw_image.data[row * raw_image.width + col] = val;
}

bool ImageHandler::checkSequence(int row, int col, unsigned int length, Byte color) const
{
    const auto first = &raw_image.data[row * raw_image.width + col];
    const auto last = &raw_image.data[row * raw_image.width + col + length];

    auto result = std::find_if(first, last, [color](unsigned char pixel)
                               { return pixel != color; });

    return result == last;
}

bool ImageHandler::isEmptyRow(int row) const
{
    return checkSequence(row, 0, raw_image.width, WHITE);
}