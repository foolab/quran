// -*- qml-mode -*-
import QtQuick 1.0

Dialog {
        id: dialog

        faderOpacity: 0.3

        property ListModel model: ListModel {}

        property alias titleText: title.text

        property int selectedIndex: -1

        property Component delegate: Component {
                id: delegate
                Rectangle {
                        id: rect
                        width: label.width
                        height: label.height * 2
                        color: index == dialog.selectedIndex ? "steelblue" : "white"

                        Label {
                                id: label
                                width: view.width
                                text: name
                                anchors.verticalCenter: parent.verticalCenter
                                color: index == dialog.selectedIndex ? "white" : "black"
                                x: 10
                        }

                        MouseArea {
                                anchors.fill: parent
                                onPressed: selectedIndex = index;
                                onClicked: accept();
                        }
                }
        }

        title: Rectangle {
                id: title
                property string text

                width: parent.width * 0.9
                height: label.height

                anchors.horizontalCenter: parent.horizontalCenter
                color: "steelblue"

                Label {
                        id: label
                        width: parent.width
                        color: "white";
                        text: parent.text
                        x: 10
                        anchors.verticalCenter: parent.verticalCenter
                }
        }

        content: ListView {
                id: view
                clip: true
                width: title.width
                anchors.horizontalCenter: parent.horizontalCenter
                model: dialog.model
                delegate: dialog.delegate
                height: Math.min(contentItem.height, dialog.height * 0.75)
                Component.onCompleted: parent.anchors.topMargin = 0; // HACK
        }
}
