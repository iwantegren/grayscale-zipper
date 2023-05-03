#include <QThread>
#include <QFileInfo>

class ZipperWrapper : public QThread
{
    Q_OBJECT

public:
    ZipperWrapper(const QFileInfo &file_info);

signals:
    void wrongFile();
    void resultReady();

protected:
    void run() override;
    void encode();
    void decode();

private:
    const QFileInfo &file;
};
