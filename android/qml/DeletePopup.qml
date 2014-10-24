// -*- qml -*-
import QtQuick 2.2
import QtQuick.Dialogs 1.2

Item {
    id: item
    property QtObject dialog

    signal confirmed
    property string _question

    Component {
        id: dialogComponent

        MessageDialog {
            text: item._question
            standardButtons: StandardButton.Ok | StandardButton.Cancel
            onAccepted: item.confirmed()
        }
    }

    function confirm(message, question) {
        item._question = question

        if (item.dialog == null) {
            item.dialog = dialogComponent.createObject(item)
        }

        item.dialog.open()
    }
}
