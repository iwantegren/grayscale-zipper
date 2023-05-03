#include "FileTableModel.h"
#include <iostream>

FileTableModel::FileTableModel()
{
    files.push_back({});
}

int FileTableModel::rowCount(const QModelIndex &) const
{
    return files.size();
}

int FileTableModel::columnCount(const QModelIndex &) const
{
    return FileInfo::PROPERTIES_COUNT;
}

QVariant FileTableModel::data(const QModelIndex &index, int role) const
{
    switch (role)
    {
    case Qt::DisplayRole:
        return QString("%1, %2").arg(index.column()).arg(index.row());
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
    files.push_back({});
    files.push_back({});
    files.push_back({});
    files.push_back({});
    files.push_back({});
    files.push_back({});
    files.push_back({});
    files.push_back({});
    files.push_back({});

    std::cout << "Initialized " << files.size() << " rows from directory '/" << directory.toStdString() << "'\n";
}