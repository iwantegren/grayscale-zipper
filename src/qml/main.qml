import QtQuick 2.12
import QtQuick.Controls 2.12
import QtQuick.Window 2.12
import QtQml.Models 2.12

ApplicationWindow {
    id: application

    width: 720
    height: 480

    title: "Client App"
    visible: true

    Dialog {
        id: errorDialog
        title: "Error..."
        modal: true

        Text {
            id: errorText
            text: "Initial text"
            anchors.centerIn: parent
        }

        standardButtons: Dialog.Ok
        x: (parent.width - width) / 2
        y: (parent.height - height) / 2

        onAccepted: {
            console.log("And of course you could only agree.")
            Qt.quit()
        }

        function error(t)
        {
            errorText.text = t;
            visible = true;
        }

        Component.onCompleted: {
        }
    }
}

// Rectangle {
//     width: 400
//     height: 400
//     color: "lightgray"

//     FileDialog {
//         id: fileDialog
//         title: "Open File"
//         visible: false

//         onAccepted: {
//             console.log("Selected file: " + fileDialog.fileUrl.toString());
//         }
//     }

//     MouseArea {
//         anchors.fill: parent
//         onClicked: fileDialog.visible = true
//     }
// }
