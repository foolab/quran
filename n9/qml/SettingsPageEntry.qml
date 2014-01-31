// -*- qml -*-
import QtQuick 1.0

Rectangle {
        id: entry

        signal clicked
        signal accepted

        property alias title: titleLabel.text
        property alias subtitle: subtitleLabel.text
        property alias selectedIndex: dialog.selectedIndex

        property list<QtObject> entries

        color: mouse.pressed ? colors.pressedColor : colors.backgroundColor

        width: parent ? parent.width: undefined

        height: col.height * 1.5

        Column {
                id: col

                width: parent.width
                anchors.centerIn: parent

                Label {
                        id: titleLabel
                        width: parent.width - 20
                        font.bold: true
                        x: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: colors.textColor
                }

                Label {
                        id: subtitleLabel
                        width: parent.width - 20
                        x: 10
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: colors.textColor
                }
        }

        MouseArea {
                id: mouse
                anchors.fill: parent
                onClicked: entries.length == 0 ? entry.clicked() : dialog.open();
        }

        SelectionDialog {
                id: dialog

                parent: settingsPage

                onAccepted: entry.accepted();

                titleText: entry.title

                model: ListModel {
                        Component.onCompleted: {
                                for (var x = 0; x < entry.entries.length; x++) {
                                        append({"name": entry.entries[x].name});
                                }
                        }
                }
        }
}
