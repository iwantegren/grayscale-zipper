#include "ZipperWrapper.h"
#include <iostream>

ZipperWrapper::ZipperWrapper(const QFileInfo &file_info)
    : file(file_info)
{
}

void ZipperWrapper::run()
{
    auto extension = file.suffix();
    if (extension == "bmp")
        encode();
    else if (extension == "barch")
        decode();
    else
        emit wrongFile();
}

void ZipperWrapper::encode()
{
    std::cout << "Start encoding of '" << file.fileName().toStdString() << "' file...\n";

    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Encoding... " << i << std::endl;
        sleep(1);
    }
}

void ZipperWrapper::decode()
{
    std::cout << "Start decoding of '" << file.fileName().toStdString() << "' file...\n";

    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Decoding... " << i << std::endl;
        sleep(1);
    }
}