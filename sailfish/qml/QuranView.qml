// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0
import "QuranView.js" as QV

SilicaFlickable {
        id: flick
        property int page: index
        property Item item

        signal scrollRequest

        anchors.leftMargin: 10
        anchors.rightMargin: 10
        contentHeight: upper.height + column.height

        // Needed to make flicking reliable with SilicaFlickable
        pressDelay: 0

        // Work around a null pointer dereference in Silica.
        interactive: ListView.isCurrentItem
        Component.onCompleted: scrollRequest()

        Item {
                id: upper
                anchors.top: parent.top
                width: parent.width
                height: Math.max(verse.height, part.height)

                SuraList {
                        id: verse
                        anchors.left: parent.left
                        anchors.leftMargin: 10
                        suras: _data.surasForPage(page)
                }

                Label {
                        id: part
                        anchors.right: parent.right
                        anchors.rightMargin: 10
                        text: _data.partNameForPage(page)
                        color: _colors.textColor
                        font.family: Theme.fontFamily
                }
        }

        Column {
                id: column
                anchors.top: upper.bottom
                width: parent.width

                Repeater {
                        model: QuranViewModel {
                                page: flick.page
                                data: _data
                        }

                        delegate: Loader {
                                id: loader
                                property int _chapter: chapter
                                property int _verse: verse
                                width: parent.width
                                sourceComponent: verse == -1 ? chapterDelegate : verseDelegate
                        }
                }
        }

        Connections {
                target: pagePosition
                onChanged: scrollRequest()
        }

        onItemChanged: {
                if (item) {
                        scrollToItem(item);
                        item = null;
                }
        }

        NumberAnimation {
                id: animation
                target: flick
                from: flick.contentY
                easing.type: Easing.InOutQuad
                property: "contentY"

                function run(t) {
                        complete();
                        from = flick.contentY;
                        to = t;
                        start();
                }
        }

        function scrollToItem(iten) {
                QV.scrollToItem(item);
        }

        Component {
                id: chapterDelegate

                Column {
                        anchors {
                                left: parent.left
                                right: parent.right
                                rightMargin: 16
                                leftMargin: 16
                        }

                        Image {
                                id: chapterBorderTop
                                width: parent.width
                                height: 5
                                source: "image://icon/" + theme.chapterBorder
                        }

                        Label {
                                id: title
                                width: parent.width
                                font.family: _settings.fontFamily
                                font.pixelSize: _settings.fontSize
	                            color: _colors.titleColor
                                horizontalAlignment: Text.AlignHCenter
                                text: _data.fullSuraName(_chapter);
                                Image {
                                        anchors.fill: parent
                                        source: "image://icon/" + theme.chapterBackground
                                        z: title.z - 1
                                }
                        }

                        Label {
                                id: subtitle
                                font.family: _settings.fontFamily
                                font.pixelSize: _settings.fontSize
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter
	                            color: _colors.subtitleColor
                                text: _data.hasBasmala(_chapter) ? _data.basmala : ""
                                visible: text != ""
                                Image {
                                        anchors.fill: parent
                                        source: "image://icon/" + theme.chapterBackground
                                        z: subtitle.z - 1
                                }
                        }

                        Image {
                                id: chapterBorderBottom
                                width: parent.width
                                height: 5
                                source: "image://icon/" + theme.chapterBorder
                        }
                }
        }

        Component {
                id: verseDelegate
                Column {
                        id: col
                        anchors {
                                left: parent.left
                                right: parent.right
                                rightMargin: 16
                                leftMargin: 16
                        }

                        QuranVerseLabel {
                                id: label
                                chapter: _chapter
                                verse: _verse
                                dataProvider: _data
                                numberFormatter: _formatter
                                color: _recitations.chapter == chapter && _recitations.verse == verse ? _colors.highlightColor : _colors.verseColor

                                function scrollRequest() {
                                        if (pagePosition.isValid() &&
                                            pagePosition.sura == chapter &&
                                            pagePosition.aya == verse) {
                                                flick.item = label;
                                        }
                                }

                                Component.onCompleted: {
                                        flick.scrollRequest.connect(scrollRequest);
                                }
                        }

                        Row {
                            anchors.horizontalCenter: parent.horizontalCenter

                            IconButton {
                                BookmarkItem {
                                    id: checker
                                    bookmarks: _bookmarks
                                    bookmark: _bookmarks.serialize(_chapter, _verse)
                                }

                                icon.source: checker.isBookmarked ? "image://theme/icon-m-favorite-selected?black" : "image://theme/icon-m-favorite?black"
                                onClicked: checker.toggle()
                            }

                            IconButton {
                                icon.source: "image://theme/icon-m-region?black"
                                visible: _settings.translationMode == 2 && _fsmon.available
                                onClicked: translation.shown = !translation.shown;
                            }

                            IconButton {
                                icon.source: "image://theme/icon-m-play?black"
                                visible: _settings.recitationMode != 0 && _fsmon.available
                                onClicked: _recitations.play(_chapter, _verse);
                            }
                        }

                        QuranTranslationLabel {
                                id: translation
                                chapter: _chapter
                                verse: _verse
                        }
                }
        }
}
