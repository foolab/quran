// -*- qml -*-
import QtQuick 1.0
import Label2 1.0

Menu {
        id: contextMenu
        property variant bookmark: 0
        property int chapter: -1
        property int verse: -1
        property alias text: ayaText.text
        property alias label: item.text

        signal clicked

        MenuLayout {
                Rectangle {
                        width: item.width
                        height: ayaText.height + 20

                        Label2 {
                                id: ayaText
                                width: parent.width - 40
                                font.pointSize: 26
                                font.family: _settings.fontFamily
                                anchors.horizontalCenter: parent.horizontalCenter
                                anchors.verticalCenter: parent.verticalCenter
                        }
                }

                MenuItem { id: item; onClicked: contextMenu.clicked(); }
                // TODO: mass storage mode.
                MenuItem { text: _recitations.installed.length == 0 ? "" : qsTr("Recite"); onClicked: { _recitations.play(contextMenu.chapter, contextMenu.verse); contextMenu.close(); } }
        }
}
