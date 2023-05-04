#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <iostream>
#include "FileTableModel.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto directory = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments().at(1) : QString("");
    std::cout << directory.toStdString() << std::endl;

    FileTableModel model(directory);
    model.lookup();

    QQuickView *view = new QQuickView;
    view->rootContext()->setContextProperty("directory", directory);
    view->rootContext()->setContextProperty("filetablemodel", &model);
    view->setSource(QUrl("qrc:/main.qml"));
    view->show();

    return app.exec();
}