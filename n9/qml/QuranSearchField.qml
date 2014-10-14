// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

TextField {
        property bool enterKeyEnabled
        property string enterKeyText

        signal enterKeyClicked

//        textAlignment: Text.AlignRight

        Keys.onReturnPressed: enterKeyClicked()
// TODO:
//        EnterKey.enabled: enterKeyEnabled
//        EnterKey.text: enterKeyText
}
