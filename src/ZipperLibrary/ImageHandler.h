#pragma once

#include "Misc.h"

class ImageHandler
{
public:
    ImageHandler(RawImageData &image);

    Byte get(int row, int col) const;
    void set(int row, int col, Byte val);

    bool checkSequence(int row, int col, unsigned int length, Byte color) const;

    bool isEmptyRow(int row) const;

    int width() const { return raw_image.width; };
    int heigth() const { return raw_image.height; };

    static const Byte WHITE = 0xFF;
    static const Byte BLACK = 0x00;

protected:
    RawImageData &raw_image;
};