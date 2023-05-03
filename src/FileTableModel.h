#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include <QString>
#include <vector>

struct FileInfo
{
    QString name;
    float size;

    static const int PROPERTIES_COUNT = 2;
};

class FileTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileTableModel();

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

public slots:
    void initialize(const QString &directory);

private:
    std::vector<FileInfo> files;
};
