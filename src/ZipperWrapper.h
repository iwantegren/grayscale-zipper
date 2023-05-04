#pragma once

#include <QThread>
#include <QFileInfo>

enum class Status;

class ZipperWrapper : public QThread
{
    Q_OBJECT

public:
    ZipperWrapper(const QFileInfo &file_info, Status action);

signals:
    void wrongFile();
    void resultReady(const QString &filename);

protected:
    void run() override;
    void encode();
    void decode();

private:
    const QFileInfo &file;
    Status action;
};
