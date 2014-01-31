// -*- qml -*-
import QtQuick 1.0

Label {
        Rectangle {
                anchors.fill: parent
                color: colors.backgroundColor
                z: label.z - 1
        }

        id: label
        property int verse: -1
        property int chapter: -1
        onChapterChanged: populate();
        onVerseChanged: populate();

        width: parent.width

        signal clicked

        font.family: settings.fontFamily
        font.pointSize: settings.fontSize
	    color: colors.verseColor
        horizontalAlignment: settings.centerText ? Text.AlignHCenter : Text.AlignHRight

        MouseArea {
                anchors.fill: parent
                onClicked: label.clicked();
        }

        function populate() {
                if (label.chapter != -1 && label.verse != -1) {
                text = quranData.text(label.chapter, label.verse)
                       + " (" + formatter.number(label.verse + 1) + ")";
                }
        }

        Component.onCompleted: {
                settings.textTypeChanged.connect(populate);
                settings.numberFormatChanged.connect(populate);
                label.populate();
        }
}
