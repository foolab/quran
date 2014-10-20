// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Column {
        property alias value: slider.value
        property alias minimumValue: slider.minimumValue
        property alias maximumValue: slider.maximumValue
        property alias stepSize: slider.stepSize
        property bool valueText //: slider.valueIndicatorText
        property alias label: textLabel.text

        Label {
                id: textLabel

                anchors {
                    left: parent.left
                    right: parent.right
                    leftMargin: quranTheme.marginSmall
                    rightMargin: quranTheme.marginSmall
                }

                font.bold: true
                font.pixelSize: quranTheme.fontSizeSmall
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

//                valueIndicatorVisible: true
                updateValueWhileDragging: true
        }
}

//TODO:
