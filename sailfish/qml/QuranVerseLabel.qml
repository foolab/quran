// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Label {
        id: label
        property int verse: -1
        property int chapter: -1
        onChapterChanged: populate();
        onVerseChanged: populate();
        wrapMode: Text.WordWrap

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
                        text = qsTr("%1 (%2)").arg(_data.text(label.chapter, label.verse)).arg(_formatter.number(label.verse + 1))
                }
        }

        Component.onCompleted: {
                _data.basmalaChanged.connect(populate);
                _settings.numberFormatChanged.connect(populate);
                label.populate();
        }
}
