#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include <QString>
#include <vector>
#include <QFileInfo>

enum class Status
{
    NONE,
    ENCODING,
    DECODING
};

struct FileStatus
{
    QFileInfo info;
    Status status = Status::NONE;
};

class FileTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileTableModel(const QString &directory);

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void lookup();

public slots:
    void onRowClicked(int row);
    void onResultReady(const QString &filename, const QString error);

signals:
    void errorOccured(const QString &message);

private:
    int findRow(const QString &filename);

    std::vector<FileStatus> files;
    const QString &directory;
};
