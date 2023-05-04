#include "FileTableModel.h"
#include <iostream>
#include <QDir>
#include <QStringList>
#include "ZipperWrapper.h"
#include <algorithm>

namespace
{
    enum Columns : int
    {
        NAME = 0,
        SIZE = 1,
        STATUS = 2
    };

    static const int PROPERTIES_COUNT = 3;

    QString statusToStr(Status s)
    {
        switch (s)
        {
        case Status::ENCODING:
            return QString("Encoding...");
        case Status::DECODING:
            return QString("Decoding...");
        default:
            return QString("");
        }
    }
}

FileTableModel::FileTableModel(const QString &directory)
    : directory(directory)
{
}

int FileTableModel::rowCount(const QModelIndex &) const
{
    return files.size();
}

int FileTableModel::columnCount(const QModelIndex &) const
{
    return PROPERTIES_COUNT;
}

QVariant FileTableModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        switch (index.column())
        {
        case NAME:
            return files[index.row()].info.fileName();
        case SIZE:
            return QString("%1 b").arg(files[index.row()].info.size());
        case STATUS:
            return statusToStr(files[index.row()].status);
        default:
            break;
        }
    default:
        break;
    }

    return QVariant();
}

QHash<int, QByteArray> FileTableModel::roleNames() const
{
    return {{Qt::DisplayRole, "display"}};
}

void FileTableModel::lookup()
{
    QDir dir(directory);
    QFileInfoList files_in_dir = dir.entryInfoList(QStringList() << "*.bmp"
                                                                 << "*.barch"
                                                                 << "*.png",
                                                   QDir::Files);
    for (const QFileInfo &file : files_in_dir)
    {
        auto it = std::find_if(files.cbegin(), files.cend(), [file](const FileStatus &f)
                               { return file.fileName() == f.info.fileName(); });
        if (it == files.cend())
        {
            std::cout << "[" << file.fileName().toStdString() << " | " << file.size() << " b]\n";
            files.push_back({file, Status::NONE});
        }
    }

    emit layoutChanged();
}

void FileTableModel::onRowClicked(int row)
{
    if (files[row].status != Status::NONE)
    {
        std::cout << "File '" << files[row].info.fileName().toStdString() << "' is still processing, please wait\n";
        return;
    }

    auto extension = files[row].info.suffix();
    Status action;
    if (extension == "bmp")
        action = Status::ENCODING;
    else if (extension == "barch")
        action = Status::DECODING;
    else
    {
        std::cout << "Wrong file to process '" << files[row].info.fileName().toStdString() << "'\n";
        emit wrongFile(QString("Wrong file '%1'").arg(files[row].info.fileName()));
        return;
    }

    files[row].status = action;

    emit dataChanged(index(row, STATUS), index(row, STATUS));

    auto *zipper = new ZipperWrapper(files[row].info, action);
    connect(zipper, &ZipperWrapper::resultReady, this, &FileTableModel::onResultReady);
    zipper->start();
}

void FileTableModel::onResultReady(const QString &filename)
{
    auto it = std::find_if(files.begin(), files.end(), [filename](const FileStatus &f)
                           { return f.info.fileName() == filename; });

    if (it != files.end())
    {
        int row = it - files.begin();
        std::cout << "File [" << filename.toStdString() << "] is ready!\n";
        files[row].status = Status::NONE;
        emit dataChanged(index(row, STATUS), index(row, STATUS));

        resultReady(row);
    }

    lookup();
}
