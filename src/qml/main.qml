import QtQuick 2.12
import FileTableModel 1.0

Rectangle {
    width: 400
    height: 400
    color: "lightgray"

    Text {
        text: directory
        font.family: "Helvetica"
        font.pointSize: 24
    }

    TableView {
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: FileTableModel {}

        delegate: Rectangle {
            implicitWidth: 100
            implicitHeight: 50
            Text {
                text: display
            }
        }
    }
}
