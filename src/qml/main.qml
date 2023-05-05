import QtQuick 2.12

Rectangle {
    id: mainWindow
    width: 1500
    height: 800

    color: "lightgray"

    TableView {
        id: tableview
        anchors.fill: parent
        columnSpacing: 1
        rowSpacing: 1
        clip: true

        model: filetablemodel

        delegate: Rectangle {
            implicitWidth: 500
            implicitHeight: 28
            Text {
                text: display
                width: parent.implicitWidth
                font.pointSize: 14
                elide: Text.ElideMiddle
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    if (!errorDialog.visible)
                    {
                        filetablemodel.onRowClicked(model.row)
                    }
                }
            }
        }
    }

    Rectangle {
        id: grayBackground
        color: "#aaaaaa"
        anchors.fill: parent
        opacity: 0.6
        visible: false
    }

    Rectangle {
        id: errorDialog
        width: 400
        height: 200
        color: "#ffffff"
        border.color: "#aaaaaa"
        anchors.centerIn: parent

        visible: false

        Text {
            id: errorText
            width: errorDialog.width
            text: "Unexpected error!"
            font.pixelSize: 20
            anchors.centerIn: parent
            wrapMode: Text.Wrap
            horizontalAlignment: Text.AlignHCenter
        }

        Rectangle {
            id: okButton
            width: 80
            height: 40
            color: "#007aff"
            radius: 10
            border.width: 2
            border.color: "#000000"

            Text {
                text: "OK"
                font.pixelSize: 16
                color: "#ffffff"
                anchors.centerIn: parent
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    errorDialog.visible = false
                    grayBackground.visible = false
                    okButton.color = "#007aff"
                }
                onPressed: {
                    okButton.color = "#00c210"
                }
            }

            anchors {
                horizontalCenter: parent.horizontalCenter
                bottom: parent.bottom
                bottomMargin: 20
            }
        }

        function onErrorOccured(message)
        {
            errorText.text = message
            errorDialog.visible = true
            grayBackground.visible = true
        }

        Connections {
            target: filetablemodel
            onErrorOccured: {
                errorDialog.onErrorOccured(message);
            }
        }

    }
}
