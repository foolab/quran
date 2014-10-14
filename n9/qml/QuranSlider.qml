// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Column {
        property alias value: slider.value
        property alias minimumValue: slider.minimumValue
        property alias maximumValue: slider.maximumValue
        property alias stepSize: slider.stepSize
        property alias valueText: slider.valueIndicatorText
        property alias label: textLabel.text

        Label {
                id: textLabel
                width: parent.width * 0.9
                x: quranTheme.marginSmall
                font.bold: true
                font.pixelSize: 26
        }

        Slider {
                id: slider
                width: parent.width * 0.9
        }
}
