// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Column {
    id: column

    property int value
    property alias minimumValue: slider.minimumValue
    property alias maximumValue: slider.maximumValue
    property alias stepSize: slider.stepSize
    property bool valueText //: slider.tickmarksEnabled //TODO: Not supported
    property alias label: textLabel.text

    QuranLabel {
        id: textLabel

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        font.bold: true
        font.pixelSize: quranTheme.fontSizeSmall
        color: quranTheme.primaryColor
    }

    Slider {
        id: slider

        anchors {
            left: parent.left
            right: parent.right
            leftMargin: quranTheme.marginSmall
            rightMargin: quranTheme.marginSmall
        }

        height: quranTheme.itemSizeSmall
        stepSize: 1.0

        property bool _completed: false

        // This is a hack because Slider likes to reset its value on creation
        Component.onCompleted: {
            slider.value = column.value
            _completed = true
        }

        onValueChanged: {
            if (_completed) {
                column.value = value
            }
        }
    }
}
