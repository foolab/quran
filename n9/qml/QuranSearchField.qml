// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

TextField {
        id: field

        property bool enterKeyEnabled: true
        property string enterKeyText

        signal enterKeyClicked

        platformSipAttributes: SipAttributes {
                actionKeyHighlighted: true
                actionKeyLabel: field.enterKeyText
                actionKeyEnabled: field.enterKeyEnabled
        }

        Keys.onReturnPressed: enterKeyClicked()
}
