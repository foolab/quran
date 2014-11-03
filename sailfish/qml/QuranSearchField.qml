// -*- qml -*-
import Sailfish.Silica 1.0
import QtQuick 2.0

SearchField {
    property bool enterKeyEnabled: true
    property string enterKeyText

    signal enterKeyClicked

    horizontalAlignment: TextInput.AlignRight
    EnterKey.onClicked: enterKeyClicked()
    EnterKey.enabled: enterKeyEnabled
    EnterKey.text: enterKeyText
}
