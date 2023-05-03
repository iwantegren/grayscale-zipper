#pragma once

#include <qqml.h>
#include <QAbstractTableModel>
#include <QString>
#include <vector>
#include <QFileInfo>

class FileTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileTableModel();

    int rowCount(const QModelIndex & = QModelIndex()) const override;
    int columnCount(const QModelIndex & = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;
    QHash<int, QByteArray> roleNames() const override;

    void initialize(const QString &directory);

public slots:
    void onRowClicked(int row);

signals:
    void wrongFile();
    void resultReady();

private:
    std::vector<QFileInfo> files;
};
