// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

TextField {
        property bool enterKeyEnabled
        property string enterKeyText
        property bool enableSoftwareInputPanel

        signal enterKeyClicked

        horizontalAlignment: TextInput.AlignHCenter

//        EnterKey.onClicked: enterKeyClicked()
//        EnterKey.enabled: enterKeyEnabled
//        EnterKey.text: enterKeyText
}
//TODO:
