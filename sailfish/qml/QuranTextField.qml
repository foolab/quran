// -*- qml -*-
import Sailfish.Silica 1.0
import QtQuick 2.0

TextField {
    property bool enterKeyEnabled: true
    property string enterKeyText

    signal enterKeyClicked

    horizontalAlignment: TextInput.AlignHCenter

    EnterKey.onClicked: enterKeyClicked()
    EnterKey.enabled: enterKeyEnabled
    EnterKey.text: enterKeyText
}
