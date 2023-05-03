#include <qqml.h>
#include <QAbstractTableModel>

class FileTableModel : public QAbstractTableModel
{
    Q_OBJECT

public:
    FileTableModel()
    {
        qmlRegisterType<FileTableModel>("FileTableModel", 1, 0, "FileTableModel");
    }

    int rowCount(const QModelIndex & = QModelIndex()) const override
    {
        return 200;
    }

    int columnCount(const QModelIndex & = QModelIndex()) const override
    {
        return 200;
    }

    QVariant data(const QModelIndex &index, int role) const override
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

    QHash<int, QByteArray> roleNames() const override
    {
        return {{Qt::DisplayRole, "display"}};
    }
};
