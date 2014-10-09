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

    anchors {
        leftMargin: theme.marginSmall
        rightMargin: theme.marginSmall
    }

    header: Item {
        width: parent.width
        height: Math.max(verse.height, part.height)

        SuraList {
            id: verse
            anchors.left: parent.left
            anchors.leftMargin: theme.marginSmall
            suras: _data.surasForPage(page)
        }

        QuranLabel {
            id: part
            anchors.right: parent.right
            anchors.rightMargin: theme.marginSmall
            text: _data.partNameForPage(page)
            color: theme.textColor
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
                color: theme.chapterBorder
                height: childRect.height + 10

                anchors {
                    left: parent ? parent.left : undefined
                    right: parent ? parent.right : undefined
                    rightMargin: theme.marginMedium
                    leftMargin: theme.marginMedium
                }

                Rectangle {
                    id: childRect
                    anchors {
                        left: parent.left
                        right: parent.right
                    }

                    height: column.height
                    y: 5
                    color: theme.chapterBackground

                    Column {
                        id: column
                        width: parent.width

                        QuranLabel {
                            id: title
                            width: parent.width
                            font.family: settings.fontFamily
                            font.pixelSize: settings.fontSize
                            color: theme.titleColor
                            horizontalAlignment: Text.AlignHCenter
                            text: _data.fullSuraName(_chapter);
                        }

                        QuranLabel {
                            id: subtitle
                            font.family: settings.fontFamily
                            font.pixelSize: settings.fontSize
                            width: parent.width
                            horizontalAlignment: Text.AlignHCenter
                            color: theme.subtitleColor
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
                    rightMargin: theme.marginMedium
                    leftMargin: theme.marginMedium
                }

                QuranVerseLabel {
                    id: label
                    chapter: _chapter
                    verse: _verse
                    dataProvider: _data
                    color: _recitations.chapter == chapter && _recitations.verse == verse ? theme.highlightColor : theme.verseColor
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter

                    ToolButton {
                        BookmarkItem {
                            id: checker
                            bookmarks: _bookmarks
                            bookmark: _bookmarks.serialize(_chapter, _verse)
                        }

                        width: theme.toolButtonSize
                        icon.source:checker.isBookmarked ? highlight ? "image://icon/favorite-selected.png?" + theme.buttonHighlightColor : "image://icon/favorite-selected.png?" + theme.buttonNormalColor : highlight ? "image://icon/favorite-unselected.png?" + theme.buttonHighlightColor : "image://icon/favorite-unselected.png?" + theme.buttonNormalColor
                        onClicked: checker.toggle()
                    }

                    ToolButton {
                        width: theme.toolButtonSize
                        icon.source: highlight ? "image://icon/translation.png?" + theme.buttonHighlightColor : "image://icon/translation.png?" + theme.buttonNormalColor
                        visible: settings.translationMode == 2
                        onClicked: translation.shown = !translation.shown;
                    }

                    ToolButton {
                        width: theme.toolButtonSize
                        icon.source: highlight ? "image://icon/play.png?" + theme.buttonHighlightColor : "image://icon/play.png?" + theme.buttonNormalColor
                        visible: settings.recitationMode != 0 && _recitations.installedCount > 0
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
