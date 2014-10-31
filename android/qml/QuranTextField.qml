// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

TextField {
    id: field
    property bool enterKeyEnabled: true
    property string enterKeyText

    signal enterKeyClicked

    horizontalAlignment: TextInput.AlignHCenter
    height: quranTheme.itemSizeSmall
    Keys.onReturnPressed: enterKeyClicked()

//    EnterKey.onClicked: enterKeyClicked()
//    EnterKey.enabled: enterKeyEnabled
//    EnterKey.text: enterKeyText
}
//TODO:
