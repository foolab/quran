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
        page: view.page
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
        BorderLabel {
            horizontalAlignment: Text.AlignHCenter
            borderColor: quranTheme.chapterBorder
            backgroundColor: quranTheme.chapterBackground
            textColor: quranTheme.titleColor
            text:  qsTr("%1%2%3").arg(_data.fullSuraName(_chapter))
                .arg(_data.hasBasmala(_chapter) ? "\n" : "")
                .arg(_data.hasBasmala(_chapter) ? _data.basmala : "")
            font {
                family: settings.fontFamily
                pixelSize: settings.fontSize
            }
        }
    }

    Component {
        id: verseDelegate

        Column {
            id: column

            anchors {
                left: parent ? parent.left : undefined
                right: parent ? parent.right : undefined
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
                color: recitationPosition.chapter == _chapter && recitationPosition.verse == _verse ? quranTheme.recitationHighlightColor : quranTheme.verseColor
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
                    visible: settings.recitationMode != 0 && recitations.installedCount > 0
                    onClicked: playAudio(MediaPlayer.PlayVerse, checker.bookmark)
                }
            }

            QuranTranslationLabel {
                id: translation
                textSupplier: supplier
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
