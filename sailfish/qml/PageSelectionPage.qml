// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranPage {
    PageHeader {
        width: parent.width
        title: qsTr("Choose a page")
    }

    Column {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        Label {
            width: parent.width
            text: qsTr("Enter a page number between 1 and %1").arg(_data.pageCount)
            font {
                family: Theme.fontFamily
                pixelSize: Theme.fontSizeLarge
            }
            wrapMode: Text.WordWrap
            color: Theme.primaryColor
        }

        TextField {
            anchors.horizontalCenter: parent.horizontalCenter
            text: _settings.pageNumber + 1
            width: parent.width / 2
            focus: true
            horizontalAlignment: TextInput.AlignHCenter
            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1; top: _data.pageCount }

            font {
                pixelSize: Theme.fontSizeLarge
                family: Theme.fontFamilyHeading
            }

            EnterKey.enabled: acceptableInput
            EnterKey.text: qsTr("Go!")

            EnterKey.onClicked: {
                if (acceptableInput) {
                    _settings.pageNumber = text - 1
                    pageStack.pop()
                }
            }
        }
    }
}
