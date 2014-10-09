// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranLabel {
        id: label
        property int verse: -1
        property int chapter: -1
        property DataProvider dataProvider: null
        onChapterChanged: populate();
        onVerseChanged: populate();
        wrapMode: Text.WordWrap

        width: parent.width

        signal clicked

        font.family: settings.fontFamily
        font.pixelSize: settings.fontSize
	    color: theme.verseColor
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

        function populate() {
                if (label.chapter != -1 && label.verse != -1 && dataProvider) {
                        text = qsTr("%1 (%2)").arg(dataProvider.text(label.chapter, label.verse)).arg(formatter.formattedNumber)
                }
        }

        Component.onCompleted: {
                dataProvider.basmalaChanged.connect(populate);
                settings.numberFormatChanged.connect(populate);
                label.populate();
        }
}
