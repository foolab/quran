// -*- qml -*-
import QtQuick 1.0
import QuranViewModel 1.0
import Label2 1.0

import "QuranView.js" as QV


Flickable {
        id: flick
        clip: true
        width: parent ? parent.width : undefined

//        interactive: !_recitations.isPlaying

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

        property QuranViewModel model: QuranViewModel {data: _data}

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

                        Label2 {
                                id: title
                                width: parent.width
                                font.family: _settings.fontFamily
                                font.pointSize: _settings.fontSize
	                            color: _settings.titleColor
                                center: true

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
                                        text = _data.fullSuraName(parent.chapter);
                                }

                                Component.onCompleted: populate();
                        }

                        Label2 {
                                id: subtitle
                                font.family: _settings.fontFamily
                                font.pointSize: _settings.fontSize
                                width: parent.width
                                center: true
	                            color: _settings.subtitleColor

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

                                        if (!_data.hasBasmala(parent.chapter)) {
                                                height = 0;
                                                return;
                                        }

                                        height = undefined;
                                        text = _data.basmala();
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
//                                height: _settings.translationMode != 0 ? 5 : 0
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
                                color: _recitations.chapter == chapter && _recitations.verse == verse ? "red" : _settings.verseColor

                                onClicked: {
                                        menu.visible = !menu.visible
                                        if (!menu.visible && _settings.translationMode == 2) {
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
                                width: parent.width
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