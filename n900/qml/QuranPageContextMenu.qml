// -*- qml-mode -*-
import QtQuick 1.0

Menu {
        id: contextMenu
        property variant bookmark: 0
        property alias text: ayaText.text
        property alias label: item.text

        signal clicked

        MenuLayout {
                Rectangle {
                        width: item.width
                        height: ayaText.height

                        Label {
                                id: ayaText
                                width: parent.width - 40
                                x: 20
                                wrapMode: Text.WordWrap
                                font.pointSize: 26
                                font.family: _settings.fontFamily
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignRight
                        }
                }

                MenuItem { id: item; onClicked: contextMenu.clicked(); }
        }
}
