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
    lookup();
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
        int row = findRow(file.fileName());
        if (row == -1)
        {
            // New file
            files.push_back({file, Status::NONE});
        }
        else if (files[row].status == Status::NONE)
        {
            // Old file that is not processing now
            files[row].info = file;
        }
    }

    emit layoutChanged();
}

void FileTableModel::onRowClicked(int row)
{
    auto filename = files[row].info.fileName();

    // Check if clicked file free
    if (files[row].status != Status::NONE)
    {
        emit errorOccured(QString("File '%1' is still processing, please wait").arg(filename));
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
        std::cout << "Wrong file to process '" << filename.toStdString() << "'\n";
        emit errorOccured(QString("Wrong file '%1'").arg(filename));
        return;
    }

    // Check if result file after processing is free
    int result_file_row = findRow(ZipperWrapper::getResultFileName(filename, action));
    if (result_file_row != -1 && files[result_file_row].status != Status::NONE)
    {
        emit errorOccured(QString("File '%1' is processing, please wait").arg(files[result_file_row].info.fileName()));
        return;
    }

    files[row].status = action;

    emit dataChanged(index(row, STATUS), index(row, STATUS));

    auto *zipper = new ZipperWrapper(files[row].info, action);
    connect(zipper, &ZipperWrapper::resultReady, this, &FileTableModel::onResultReady);
    zipper->start();
}

void FileTableModel::onResultReady(const QString &filename, const QString error)
{
    int row = findRow(filename);
    if (row != -1)
    {
        files[row].status = Status::NONE;
        emit dataChanged(index(row, STATUS), index(row, STATUS));

        if (!error.isEmpty())
        {
            emit errorOccured(QString("Error during processing '%1':\n%2").arg(files[row].info.fileName()).arg(error));
        }
    }

    lookup();
}

int FileTableModel::findRow(const QString &filename)
{
    auto it = std::find_if(files.begin(), files.end(), [filename](const FileStatus &f)
                           { return f.info.fileName() == filename; });

    if (it != files.end())
        return it - files.begin();
    else
        return -1;
}