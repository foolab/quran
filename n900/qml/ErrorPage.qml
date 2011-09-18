// -*- qml -*-
import QtQuick 1.0

Page {
        id: errorPage
        tools: toolBar

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        DialogButton {
                                text: qsTr("Quit");
                                onClicked: Qt.quit();
                        }
                }
        }

        Label {
                width: parent.width - 20
                x: 10
                horizontalAlignment: Text.AlignHCenter
                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
                text: qsTr("Failed to load the Quran text. Please reinstall");
                font.bold: true
                font.pixelSize: 36
                color: "red"
                wrapMode: Text.WordWrap
        }
}
