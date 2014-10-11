// -*- qml -*-
import Sailfish.Silica 1.0

SearchField {
        property bool enterKeyEnabled
        property string enterKeyText

        signal enterKeyClicked

        EnterKey.onClicked: enterKeyClicked()
        EnterKey.enabled: enterKeyEnabled
        EnterKey.text: enterKeyText
}
