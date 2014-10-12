// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranLabel {
        id: label
        property int verse: -1
        property int chapter: -1
        property DataProvider dataProvider: null
        wrapMode: Text.WordWrap

        width: parent.width

        signal clicked

        font.family: settings.fontFamily
        font.pixelSize: settings.fontSize
	    color: theme.verseColor
        horizontalAlignment: settings.centerText ? Text.AlignHCenter : Text.AlignHRight

        TextSupplier {
                id: supplier
                chapter: label.chapter
                verse: label.verse
                data: label.dataProvider
        }

        NumberFormatter {
                id: formatter
                format: settings.numberFormat
                number: verse + 1
        }

        MouseArea {
                anchors.fill: parent
                onClicked: label.clicked();
        }

        text: qsTr("%1 (%2)").arg(supplier.primaryText).arg(formatter.formattedNumber)
}
