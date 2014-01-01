// -*- qml -*-
import QtQuick 2.0

Dialog {
        id: dialog

        faderOpacity: 0.3

        property alias model: view.model

        property alias titleText: title.text

        property int selectedIndex: -1

        property bool __hack: false

        onSelectedIndexChanged: {
                if (__hack) {
                        return;
                }

                if (selectedIndex != -1) {
                        view.positionViewAtIndex(selectedIndex, ListView.Center);
                }
        }

        Component.onCompleted: {
                if (dialog.selectedIndex != -1) {
                        view.positionViewAtIndex(selectedIndex, ListView.Center);
                }
        }

        property Component delegate: SelectionDialogRow {}

        title: Rectangle {
                id: title
                property string text

                width: parent.width * 0.9
                height: label.height

                anchors.horizontalCenter: parent.horizontalCenter
                color: _colors.dialogTitleBackgroundColor

                Label {
                        id: label
                        width: parent.width
                        color: _colors.dialogTitleTextColor
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
                delegate: dialog.delegate
                // Seems Qt 4.7.4 shipped with CSSU 16 breaks somehow if we calculate the
                // height in tems of contentItem.height.
                // height: Math.min(contentItem.height, dialog.height * 0.75)
                height: currentItem ? Math.min(currentItem.height * count, dialog.height * 0.75) : 300
                Component.onCompleted: parent.anchors.topMargin = 0; // HACK
        }
}
