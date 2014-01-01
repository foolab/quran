// -*- qml -*-
import QtQuick 2.0

Label {
        Rectangle {
                anchors.fill: parent
                color: _colors.backgroundColor
                z: label.z - 1
        }

        id: label
        property int verse: -1
        property int chapter: -1
        onChapterChanged: populate();
        onVerseChanged: populate();

        width: parent.width

        signal clicked

        font.family: _settings.fontFamily
        font.pointSize: _settings.fontSize
	    color: _colors.verseColor
        horizontalAlignment: _settings.centerText ? Text.AlignHCenter : Text.AlignHRight

        MouseArea {
                anchors.fill: parent
                onClicked: label.clicked();
        }

        function populate() {
                if (label.chapter != -1 && label.verse != -1) {
                text = _data.text(label.chapter, label.verse)
                       + " (" + _formatter.number(label.verse + 1) + ")";
                }
        }

        Component.onCompleted: {
                _settings.textTypeChanged.connect(populate);
                _settings.numberFormatChanged.connect(populate);
                label.populate();
        }
}
