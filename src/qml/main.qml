import QtQuick 2.12

Rectangle {
    width: 800
    height: 600
    color: "lightgray"

    TableView {
        id: tableview
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: filetablemodel

        delegate: Rectangle {
            implicitWidth: 300
            implicitHeight: 30
            Text {
                text: display
            }
        }
    }
}
