// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2 as Ctrls
import QtQuick.Controls.Styles 1.2

Ctrls.TextField {
    property bool enterKeyEnabled: true
    property string enterKeyText
    property bool enableSoftwareInputPanel

    signal enterKeyClicked
    height: quranTheme.itemSizeLarge
    width: parent.width * 0.75
    placeholderText: qsTr("Search")
    textColor: quranTheme.primaryColor

    style: Component {
        TextFieldStyle {
            placeholderTextColor: quranTheme.highlightColor
            padding.right: quranTheme.itemSizeSmall
            background: Rectangle {
                anchors.fill: parent
                radius: control.heighteight * 0.16
                border.color: control.activeFocus ? quranTheme.highlightColor : quranTheme.primaryColor
                color: quranTheme.backgroundColor
                Item {
                    anchors.right: parent.right
                    anchors.top: parent.top
                    height: parent.height
                    width: quranTheme.itemSizeSmall
                    ToolButton {
                        width: Math.min(parent.width, parent.height)
                        height: width
                        anchors.centerIn: parent
                        icon.source: "image://icon/search.png?" + quranTheme.primaryColor
                    }
                }
            }
        }
    }
//    EnterKey.onClicked: enterKeyClicked()
//    EnterKey.enabled: enterKeyEnabled
//    EnterKey.text: enterKeyText
}
//TODO:
