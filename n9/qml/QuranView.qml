// -*- qml -*-
import QtQuick 1.0
import Quran 1.0
import "QuranView.js" as QV

Flickable {
        id: flick
        clip: true
        width: parent ? parent.width : undefined

//        interactive: !recitations.isPlaying

        property int page: -1

        property Item item: null

        signal scrollRequest

        function show() {
                state = "shown"
        }

        function hide() {
                state = "hidden"
                flick.destroy(300);
        }

        state: "hidden"

        states: [
        State {
                name: "shown"
                PropertyChanges { target: flick; opacity: 1.0 }
                },
        State {
                name: "hidden"
                PropertyChanges { target: flick; opacity: 0.0 }
              }
        ]

        transitions: [
        Transition {
                from: "shown"; to: "hidden"
                PropertyAnimation { properties: "opacity"; duration: 250 }
        },
        Transition {
                from: "hidden"; to: "shown"
                PropertyAnimation { properties: "opacity"; duration: 250 }
        }
        ]

        Connections {
                target: pagePosition
                onChanged: scrollRequest();
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

        onPageChanged: {
                if (page != -1) {
                        model.page = page;
                        model.populate();
                        QV.populate(column);
                        scrollRequest();
                }
        }

        property QuranViewModel model: QuranViewModel {data: quranData}

        anchors.fill: parent
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        contentHeight: column.height

        Component {
                id: chapterDelegate

                Column {
                        property int chapter: -1
                        onChapterChanged: populate();

                        function populate() {
                                title.populate();
                                subtitle.populate();
                        }

                        width: parent ? parent.width : undefined

                        Image {
                                id: chapterBorderTop
                                width: parent.width
                                height: 5
                                source: "image://theme/" + theme.chapterBorder
                        }

                        Label {
                                id: title
                                width: parent.width
                                font.family: settings.fontFamily
                                font.pointSize: settings.fontSize
	                            color: colors.titleColor
                                horizontalAlignment: Text.AlignHCenter

                                Image {
                                        anchors.fill: parent
                                        source: "image://theme/" + theme.chapterBackground
                                        z: title.z - 1
                                }

                                function populate() {
                                        if (parent.chapter == -1) {
                                                height = 0;
                                                return;
                                        }

                                        height = undefined;
                                        text = quranData.fullSuraName(parent.chapter);
                                }

                                Component.onCompleted: populate();
                        }

                        Label {
                                id: subtitle
                                font.family: settings.fontFamily
                                font.pointSize: settings.fontSize
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter
	                            color: colors.subtitleColor

                                Image {
                                        anchors.fill: parent
                                        source: "image://theme/" + theme.chapterBackground
                                        z: subtitle.z - 1
                                }

                                function populate() {
                                        if (parent.chapter == -1) {
                                                height = 0;
                                                return;
                                        }

                                        if (!quranData.hasBasmala(parent.chapter)) {
                                                height = 0;
                                                return;
                                        }

                                        height = undefined;
                                        text = quranData.basmala();
                                }

                                Component.onCompleted: {
                                        settings.textTypeChanged.connect(populate);
                                        populate();
                                }
                        }

                        Image {
                                id: chapterBorderBottom
                                width: parent.width
                                height: 5
//                                height: settings.translationMode != 0 ? 5 : 0
                                source: "image://theme/" + theme.chapterBorder
                        }
                }
        }

        Component {
                id: verseDelegate
                Column {
                        id: col
                        property int chapter: -1
                        property int verse: -1
                        width: parent ? parent.width : undefined

                        QuranVerseLabel {
                                id: label
                                chapter: col.chapter
                                verse: col.verse
                                color: recitations.chapter == chapter && recitations.verse == verse ? colors.highlightColor : colors.verseColor

                                onClicked: {
                                        menu.visible = !menu.visible
                                        if (!menu.visible && settings.translationMode == 2) {
                                                translation.shown = false;
                                        }
                                }

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

                        QuranPageContextMenu {
                                id: menu
                                verse: col.verse
                                chapter: col.chapter
                        }

                        QuranTranslationLabel {
                                id: translation
                                chapter: col.chapter
                                verse: col.verse
                        }
                }
        }

        Column {
                id: column
                width: parent.width
        }
}
