// -*- qml -*-
import QtQuick 1.0
import QuranViewModel 1.0
import Label2 1.0

Flickable {
        id: flick
        clip: true

        property int page: -1

        property Item item: null

        signal scrollRequest

        signal showAddToFavoritesMenu(string text, int chapter, int verse);
        signal showRemoveFromFavoritesMenu(string text, int chapter, int verse);

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

        function showMenu(item, chapter, verse) {
                if (verse == -1 || chapter == -1) {
                        return;
                }

                if (_bookmarks.isBookmarked(chapter, verse)) {
                        showRemoveFromFavoritesMenu(item.text, chapter, verse);
                }
                else {
                        showAddToFavoritesMenu(item.text, chapter, verse);
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
                var upper = flick.mapFromItem(item, 0, item.y).y;
                var lower = flick.mapFromItem(item, 0, item.y + item.height).y;

                // Let's reset here. We have multiple exit points.
                pagePosition.reset();

                if (upper >= flick.contentY && lower <= flick.contentY + flick.height) {
                        // Nothing.
                        return;
                }
                else if (lower <= flick.contentY + flick.height) {
                        // Topmost part is not visible.
                        // We will scroll anyway and make it visible.
                        animation.run(upper);
                        return;
                }

                if (lower - upper > flick.height) {
                        // The line will not fit no matter what we do.
                        // Just show the upper part.
                        animation.run(upper);
                        return;
                }

                // Our line will fit the view. We need to scroll until the bottommost part
                // is just visible.
                var part = upper + (lower - (upper + flick.height));
                animation.run(part);
        }

        onPageChanged: {
                if (page != -1) {
                        model.page = page;
                        model.populate();
                        column.populate();
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
                        width: parent ? parent.width : undefined

                        Label2 {
                                id: title
                                width: parent.width
                                font.family: _settings.fontFamily
                                font.pointSize: _settings.fontSize
	                            color: _settings.titleColor
                                center: true

                                function populate() {
                                        if (parent.chapter == -1) {
                                                height = 0;
                                                return;
                                        }

                                        height = undefined;
                                        text = _data.fullSuraName(parent.chapter);
                                }

                                Component.onCompleted: {
                                        parent.chapterChanged.connect(populate);
                                        populate();
                                }
                        }

                        Label2 {
                                id: subtitle
                                font.family: _settings.fontFamily
                                font.pointSize: _settings.fontSize
                                width: parent.width
                                center: true
	                            color: _settings.subtitleColor

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
                                        parent.chapterChanged.connect(populate);
                                        _settings.textTypeChanged.connect(populate);
                                        populate();
                                }
                        }
                }
        }

        Component {
                id: verseDelegate

                Column {
                        id: col
                        property int verse: -1
                        property int chapter: -1
                        width: parent ? parent.width : undefined
                        property Item translation: null

                        Label2 {
                                id: verse
                                font.family: _settings.fontFamily
                                font.pointSize: _settings.fontSize
                                width: parent.width
	                            color: _settings.verseColor

                                MouseArea {
                                        anchors.fill: parent
                                        onClicked: {
                                                if (_settings.translationMode == 2) {
                                                        col.translation.toggle();
                                                }
                                        }

                                        onPressAndHold: showMenu(verse, col.chapter, col.verse)
                                }

                                function populate() {
                                        if (parent.chapter == -1 || parent.verse == -1) {
                                                return;
                                        }

                                        text = _data.text(parent.chapter, parent.verse)
                                        + " (" + _formatter.number(parent.verse + 1) + ")";
                                }

                                function scrollRequest() {
                                        if (pagePosition.isValid() &&
                                        pagePosition.sura == parent.chapter &&
                                        pagePosition.aya == parent.verse) {
                                                flick.item = verse;
                                        }
                                }

                                Component.onCompleted: {
                                        parent.chapterChanged.connect(populate);
                                        parent.verseChanged.connect(populate);
                                        _settings.textTypeChanged.connect(populate);
                                        _settings.numberFormatChanged.connect(populate);
                                        populate();
                                        flick.scrollRequest.connect(scrollRequest);
                                }
                        }

                        Column {
                                id: translationContainer
                                width: parent.width

                                Image {
                                        id: borderTop
                                        width: parent.width
                                        height: translation.height > 0 ? 5 : 0
                                        source: "image://theme/" + theme.translationBorder
                                }

                                Label2 {
                                        id: translation
                                        Image {
                                                anchors.fill: parent
                                                source: "image://theme/" + theme.translationBackground
                                                z: translation.z - 1
                                        }

                                        width: parent.width
                                        font.family: _settings.translationFontFamily
                                        font.pointSize: _settings.translationFontSize
	                                    color: _settings.verseColor

                                        function resetText() {
                                                if (col.chapter == -1 || col.verse == -1 ||
                                                    !translationsManager.enabled ||
                                                    _settings.translationMode != 1) {
                                                        clearText();
                                                        return;
                                                }

                                                addText();
                                        }

                                        function toggle() {
                                                if (height == 0) {
                                                        addText();
                                                }
                                                else {
                                                        clearText();
                                                }
                                        }

                                        function clearText() {
                                                text = "";
                                                height = 0;
                                        }

                                        function addText() {
                                                height = undefined;
                                                text = _data.secondaryText(col.chapter, col.verse);
                                        }

                                        MouseArea {
                                                anchors.fill: parent
                                                onClicked: {
                                                        if (_settings.translationMode == 2) {
                                                                translation.toggle();
                                                        }
                                                }

                                                onPressAndHold: showMenu(verse, col.chapter, col.verse);
                                        }

                                        Connections {
                                                target: translationsManager
                                                onEnabledChanged: translation.resetText();
                                        }

                                        Connections {
                                                target: col
                                                onVerseChanged: translation.resetText();
                                                onChapterChanged: translation.resetText();
                                        }

                                        Component.onCompleted: {
                                                col.translation = translation;
                                                _settings.defaultTranslationChanged.connect(resetText);
                                                resetText();
                                        }
                                }

                                Image {
                                        id: borderBottom
                                        width: parent.width
                                        height: translation.height > 0 ? 5 : 0
                                        source: "image://theme/" + theme.translationBorder
                                }
                        }
                }
        }

        Column {
                id: column
                width: parent ? parent.width : undefined

                function populate() {
                        var chapters = model.chapters();
                        for (var x = 0; x < chapters.length; x++) {
                                var chapter = chapters[x];
                                var verses = model.verses(chapters[x]);
                                if (verses[0] == 0) {
                                        var item = chapterDelegate.createObject(column);
                                        item.chapter = chapter;
                                }

                                for (var i = 0; i < verses.length; i++) {
                                        var verse = verses[i];
                                        var item = verseDelegate.createObject(column);
                                        item.chapter = chapter;
                                        item.verse = verse;
                                }
                        }
                }
        }
}
