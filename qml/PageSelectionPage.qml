// -*- qml -*-
import QtQuick 2.0

QuranPage {
    QuranPageHeader {
        width: parent.width
        title: qsTr("Choose a page")
    }

    Column {
        width: parent.width
        anchors.verticalCenter: parent.verticalCenter

        QuranLabel {
            width: parent.width
            text: qsTr("Enter a page number between 1 and %1").arg(_data.pageCount)
            font.pixelSize: theme.fontSizeLarge
            wrapMode: Text.WordWrap
            color: theme.primaryColor
        }

        QuranTextField {
            anchors.horizontalCenter: parent.horizontalCenter
            text: settings.pageNumber + 1
            width: parent.width / 2
            focus: true
            inputMethodHints: Qt.ImhDigitsOnly
            validator: IntValidator { bottom: 1; top: _data.pageCount }

            font {
                pixelSize: theme.fontSizeLarge
                family: theme.fontFamilyHeading
            }

            enterKeyEnabled: acceptableInput
            enterKeyText: qsTr("Go!")
            onEnterKeyClicked: {
                if (acceptableInput) {
                    settings.pageNumber = text - 1
                    popPage()
                }
            }
        }
    }
}
