#include "FileTableModel.h"
#include <iostream>
#include <QDir>
#include <QStringList>
#include "ZipperWrapper.h"

namespace
{
    enum Columns : int
    {
        NAME,
        SIZE,
        STATUS
    };

    static const int PROPERTIES_COUNT = 2;
}

FileTableModel::FileTableModel()
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
            return files[index.row()].fileName();
        case SIZE:
            return QString("%1 b").arg(files[index.row()].size());
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

void FileTableModel::initialize(const QString &directory)
{
    QDir dir(directory);
    QFileInfoList files_in_dir = dir.entryInfoList(QStringList() << "*.bmp"
                                                                 << "*.barch"
                                                                 << "*.png",
                                                   QDir::Files);
    for (const QFileInfo &file : files_in_dir)
    {
        std::cout << "[" << file.fileName().toStdString() << " | " << file.size() << " b]\n";
        files.push_back(file);
    }

    std::cout << "Initialized " << files.size() << " rows from directory '" << directory.toStdString() << "'\n";
}

void FileTableModel::onRowClicked(int row)
{
    ZipperWrapper *zipper = new ZipperWrapper(files[row]);
    connect(zipper, &ZipperWrapper::wrongFile, this, &FileTableModel::wrongFile);
    connect(zipper, &ZipperWrapper::resultReady, this, &FileTableModel::resultReady);
    zipper->start();
}