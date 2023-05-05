#include <QGuiApplication>
#include <QQuickView>
#include <QQmlContext>
#include <iostream>
#include "FileTableModel.h"

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);

    auto directory = QCoreApplication::arguments().size() > 1 ? QCoreApplication::arguments().at(1) : QString("");
    std::cout << directory.toStdString() << std::endl;

    FileTableModel model(directory);

    QQuickView *view = new QQuickView;
    view->rootContext()->setContextProperty("filetablemodel", &model);
    view->setSource(QUrl("qrc:/main.qml"));
    view->show();

    return app.exec();
}