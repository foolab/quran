// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranLabel {
        id: label
        property QtObject textSupplier
        property int verse
        width: parent.width

        signal clicked

        font.family: settings.fontFamily
        font.pixelSize: settings.fontSize
	    color: quranTheme.verseColor
        horizontalAlignment: settings.centerText ? Text.AlignHCenter : Text.AlignHRight

        NumberFormatter {
                id: formatter
                format: settings.numberFormat
                number: verse + 1
        }

        MouseArea {
                anchors.fill: parent
                onClicked: label.clicked();
        }

        text: qsTr("%1 (%2)").arg(textSupplier.primaryText).arg(formatter.formattedNumber)
}
