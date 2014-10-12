// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

Column {
        Component.onCompleted: populate();

        function populate() {
                title.populate();
                subtitle.populate();
        }

        Image {
                id: chapterBorderTop
                width: parent.width
                height: 5
                source: "image://theme/" + theme.chapterBorder
        }

        Label {
                id: title
                width: parent.width
                font.family: _settings.fontFamily
                font.pointSize: _settings.fontSize
	            color: _colors.titleColor
                horizontalAlignment: Text.AlignHCenter

                Image {
                        anchors.fill: parent
                        source: "image://theme/" + theme.chapterBackground
                        z: title.z - 1
                }

                function populate() {
                        text = _data.fullSuraName(chapter);
                }

                Component.onCompleted: populate();
        }

        Label {
                id: subtitle
                font.family: _settings.fontFamily
                font.pointSize: _settings.fontSize
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
	            color: _colors.subtitleColor

                Image {
                        anchors.fill: parent
                        source: "image://theme/" + theme.chapterBackground
                        z: subtitle.z - 1
                }

                function populate() {
                        if (!_data.hasBasmala(chapter)) {
                                height = 0;
                                return;
                        }

                        height = undefined;
                        text = _data.basmala
                }

                Component.onCompleted: {
                        _settings.textTypeChanged.connect(populate);
                        populate();
                }
        }

        Image {
                id: chapterBorderBottom
                width: parent.width
                height: 5
                source: "image://theme/" + theme.chapterBorder
        }
}
