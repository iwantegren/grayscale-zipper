#include "ZipperWrapper.h"
#include "FileTableModel.h"
#include <iostream>
#include <QFile>
#include <QIODevice>
#include "ZipperLibrary/include/Encoder.h"
#include "ZipperLibrary/include/Decoder.h"
#include "ZipperLibrary/include/Misc.h"
#include "ZipperLibrary/include/ZipperException.h"
#include <vector>
#include <bitset>

ZipperWrapper::ZipperWrapper(const QFileInfo &file_info, Status action)
    : file(file_info), action(action)
{
}

QString ZipperWrapper::getResultFileName(const QString &filename, Status action)
{
    const QString suffix_encoded = ".packed.barch";
    const QString suffix_decoded = ".unpacked.bmp";

    switch (action)
    {
    case Status::ENCODING:
        return filename + suffix_encoded;
    case Status::DECODING:
        return filename + suffix_decoded;
    default:
        return filename;
    }
}

void ZipperWrapper::run()
{
    switch (action)
    {
    case Status::ENCODING:
        encode();
        break;
    case Status::DECODING:
        decode();
        break;
    default:
        break;
    }
}

void ZipperWrapper::encode()
{
    std::cout << "Start encoding of '" << file.fileName().toStdString() << "' file...\n";

    sleep(1);

    QString error; // By default error message is empty
    try
    {
        auto raw_image_data = Encoder::readDecoded(file.filePath().toStdString());

        std::vector<Byte> buffer;
        Encoder::encode(raw_image_data, buffer);

        Encoder::writeEncoded(getResultFileName(file.filePath(), action).toStdString(), buffer);
    }
    catch (ZipperException e)
    {
        error = e.what();
    }

    emit resultReady(file.fileName(), error);
}

void ZipperWrapper::decode()
{
    std::cout << "Start decoding of '" << file.fileName().toStdString() << "' file...\n";

    sleep(1);

    QString error; // By default error message is empty
    try
    {
        std::vector<Byte> buffer;
        Decoder::readEncoded(file.filePath().toStdString(), buffer);

        auto raw_image_data = Decoder::decode(buffer);

        Decoder::writeDecoded(getResultFileName(file.filePath(), action).toStdString(), raw_image_data);
    }
    catch (ZipperException e)
    {
        error = e.what();
    }

    emit resultReady(file.fileName(), error);
}