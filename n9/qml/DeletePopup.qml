// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Item {
        id: item
        property Item dialog

        signal confirmed
        property string _question

        Component {
                id: dialogComponent

                QueryDialog {
                        titleText: item._question
                        acceptButtonText: qsTr("Ok")
                        rejectButtonText: qsTr("Cancel")
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
