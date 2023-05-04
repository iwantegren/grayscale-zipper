#include "ZipperWrapper.h"
#include "FileTableModel.h"
#include <iostream>
#include <QFile>
#include <QIODevice>

ZipperWrapper::ZipperWrapper(const QFileInfo &file_info, Status action)
    : file(file_info), action(action)
{
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

    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Encoding... " << i << std::endl;
        sleep(1);
    }

    QString suffix_encoded = ".packed.barch";

    QFile sourceFile(file.filePath());
    QFile destinationFile(file.filePath() + suffix_encoded);

    sourceFile.open(QIODevice::ReadOnly);
    destinationFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QByteArray buffer;
    qint64 bytesRead = 0;

    do
    {
        buffer = sourceFile.read(4096);
        bytesRead = buffer.size();
        destinationFile.write(buffer);
    } while (bytesRead > 0);

    sourceFile.close();
    destinationFile.close();

    emit resultReady(file.fileName());
}

void ZipperWrapper::decode()
{
    std::cout << "Start decoding of '" << file.fileName().toStdString() << "' file...\n";

    for (int i = 0; i < 5; ++i)
    {
        std::cout << "Decoding... " << i << std::endl;
        sleep(1);
    }

    QString suffix_decoded = ".unpacked.bmp";

    QFile sourceFile(file.filePath());
    QFile destinationFile(file.filePath() + suffix_decoded);

    sourceFile.open(QIODevice::ReadOnly);
    destinationFile.open(QIODevice::WriteOnly | QIODevice::Truncate);

    QByteArray buffer;
    qint64 bytesRead = 0;

    do
    {
        buffer = sourceFile.read(4096);
        bytesRead = buffer.size();
        destinationFile.write(buffer);
    } while (bytesRead > 0);

    sourceFile.close();
    destinationFile.close();

    emit resultReady(file.fileName());
}