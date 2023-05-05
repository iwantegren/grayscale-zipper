#pragma once

#include <QThread>
#include <QFileInfo>

enum class Status;

class ZipperWrapper : public QThread
{
    Q_OBJECT

public:
    ZipperWrapper(const QFileInfo &file_info, Status action);

    static QString getResultFileName(const QString &filename, Status action);

signals:
    void resultReady(const QString &filename, const QString error);

protected:
    void run() override;
    void encode();
    void decode();

private:
    const QFileInfo &file;
    const Status action;
};
