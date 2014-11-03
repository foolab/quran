// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

TextField {
    id: field
    property bool enterKeyEnabled: true
    property string enterKeyText

    signal enterKeyClicked

    horizontalAlignment: TextInput.AlignHCenter
    height: quranTheme.itemSizeSmall
    Keys.onReturnPressed: enterKeyClicked()
    textColor: quranTheme.primaryColor

    style: Component {
        TextFieldStyle {
            placeholderTextColor: quranTheme.highlightColor

            background: Rectangle {
                anchors.fill: parent
                radius: control.heighteight * 0.16
                border.color: control.activeFocus ? quranTheme.highlightColor : quranTheme.primaryColor
                color: quranTheme.backgroundColor
            }
        }
    }

//TODO:
//    EnterKey.onClicked: enterKeyClicked()
//    EnterKey.enabled: enterKeyEnabled
//    EnterKey.text: enterKeyText
}
