import QtQuick 2.12

Rectangle {
    width: 600
    height: 300

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
            implicitHeight: 36
            Text {
                text: display
                font.pointSize: 20
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    filetablemodel.onRowClicked(model.row)
                }
            }
        }
    }
}
