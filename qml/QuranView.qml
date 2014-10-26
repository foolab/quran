// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranListView {
    property int page: index
    id: view
    width: ListView.view.width
    height: ListView.view.height

    // Work around a null pointer dereference in Silica.
    interactive: ListView.isCurrentItem
    highlightFollowsCurrentItem: true
    highlightMoveDuration: 500

    // Needed to make flicking reliable with SilicaFlickable
    pressDelay: 0

    header: Item {
        width: parent.width
        height: Math.max(verse.height, part.height)

        SuraList {
            id: verse
            anchors.left: parent.left
            anchors.leftMargin: quranTheme.marginSmall
            suras: _data.surasForPage(page)
        }

        QuranLabel {
            id: part
            anchors.right: parent.right
            anchors.rightMargin: quranTheme.marginSmall
            text: _data.partNameForPage(page)
            color: quranTheme.textColor
        }
    }

    model: QuranViewModel {
        data: _data
        page: index
    }

    delegate: Loader {
        id: loader
        property int _chapter: chapter
        property int _verse: verse
        width: parent.width
        sourceComponent: verse == -1 ? chapterDelegate : verseDelegate
    }

    Component {
        id: chapterDelegate

        Item {
            height: rect.height
            Rectangle {
                id: rect
                color: quranTheme.chapterBorder
                height: childRect.height + 10

                anchors {
                    left: parent ? parent.left : undefined
                    right: parent ? parent.right : undefined
                    rightMargin: quranTheme.marginSmall
                    leftMargin: quranTheme.marginSmall
                }

                Rectangle {
                    id: childRect
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: column.height
                    y: 5
                    color: quranTheme.chapterBackground

                    Column {
                        id: column
                        width: parent.width

                        QuranLabel {
                            id: title
                            width: parent.width
                            font.family: settings.fontFamily
                            font.pixelSize: settings.fontSize
                            color: quranTheme.titleColor
                            horizontalAlignment: Text.AlignHCenter
                            text: _data.fullSuraName(_chapter);
                        }

                        QuranLabel {
                            id: subtitle
                            font.family: settings.fontFamily
                            font.pixelSize: settings.fontSize
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            color: quranTheme.subtitleColor
                            text: _data.hasBasmala(_chapter) ? _data.basmala : ""
                            visible: text != ""
                        }
                    }
                }
            }
        }
    }

    Component {
        id: verseDelegate

        Item {
            height: column.height
            Column {
                id: column

                anchors {
                    left: parent.left
                    right: parent.right
                    rightMargin: quranTheme.marginSmall
                    leftMargin: quranTheme.marginSmall
                }

                TextSupplier {
                    id: supplier
                    chapter: _chapter
                    verse: _verse
                    data: _data
                }

                QuranVerseLabel {
                    id: label
                    width: parent.width
                    color: _recitations.chapter == _chapter && _recitations.verse == _verse ? quranTheme.recitationHighlightColor : quranTheme.verseColor
                    textSupplier: supplier
                    verse: _verse
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: quranTheme.spacing

                    ToolButton {
                        BookmarkItem {
                            id: checker
                            bookmarks: _bookmarks
                            bookmark: _bookmarks.serialize(_chapter, _verse)
                        }

                        icon.source:checker.isBookmarked ? highlight ? "image://icon/favorite-selected.png?" + quranTheme.buttonHighlightColor : "image://icon/favorite-selected.png?" + quranTheme.buttonNormalColor : highlight ? "image://icon/favorite-unselected.png?" + quranTheme.buttonHighlightColor : "image://icon/favorite-unselected.png?" + quranTheme.buttonNormalColor
                        onClicked: checker.toggle()
                    }

                    ToolButton {
                        icon.source: highlight ? "image://icon/translation.png?" + quranTheme.buttonHighlightColor : "image://icon/translation.png?" + quranTheme.buttonNormalColor
                        visible: settings.translationsHidden
                        onClicked: translation.shown = !translation.shown;
                    }

                    ToolButton {
                        icon.source: highlight ? "image://icon/play.png?" + quranTheme.buttonHighlightColor : "image://icon/play.png?" + quranTheme.buttonNormalColor
                        visible: settings.recitationMode != 0 && _recitations.installedCount > 0
                        onClicked: _recitations.play(_chapter, _verse);
                    }
                }

                QuranTranslationLabel {
                    id: translation
                    chapter: _chapter
                    verse: _verse
                    textSupplier: supplier
                }
            }
        }
    }

    Connections {
        target: pagePosition
        onChanged: scrollRequest()
    }

    function scrollRequest() {
        if (!pagePosition.isValid() ||
            index != _data.pageNumberForSuraAndAya(pagePosition.sura, pagePosition.aya)) {
            // Not for us.
            return;
        }

        var target = view.model.findIndex(pagePosition.sura, pagePosition.aya)
        if (target != -1) {
            view.currentIndex = target
        }
    }
}
