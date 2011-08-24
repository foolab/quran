// -*- qml-mode -*-
import QtQuick 1.0
import Quran 1.0

Page {

// TODO: reset flick when we enter full screen (when we are at the end of the page).
        id: quranPage
        tools: toolBar

        TranslationSelector {
                id: translationSelector
        }

        QuranPageContextMenu {
                id: addFavorite
                label: qsTr("Add to favorites");
                onClicked: { _bookmarks.add(bookmark); addFavorite.close(); }
        }

        QuranPageContextMenu {
                id: rmFavorite
                label: qsTr("Remove from favorites");
                onClicked: { _bookmarks.remove(bookmark); rmFavorite.close(); }
        }

        Component {
                id: quranPageDelegate

                QuranView {
                        id: content
                        font.pointSize: _settings.fontSize
                        font.family: _settings.fontFamily

                        width: view.width
                        margin: 20
                        dataProvider: _data
                        bookmarks: _bookmarks
                        formatter: _formatter
                        highlightColor: _settings.highlightColor
	                    verseColor: _settings.verseColor
	                    titleColor: _settings.titleColor
	                    subtitleColor: _settings.subtitleColor

                        Connections {
                                target: _settings
                                onNumberFormatChanged: populate();
                                onTextTypeChanged: populate();
                        }

                        Connections {
                                target: pagePosition
                                onChanged: scrollTo(pagePosition.sura, pagePosition.aya);
                        }

                        Connections {
                                target: _fsmon
                                onAvailableChanged: {
                                        if (_fsmon.available) {
                                                content.addSecondaryText = _settings.translationMode == 1
                                                if (refreshTranslations(true)) {
                                                        populate();
                                                }
                                        }
                                        else {
                                                _translations.unload();
                                        }
                                }
                        }

                        Connections {
                                target: _settings

                                onTranslationModeChanged: {
                                        content.addSecondaryText = _settings.translationMode == 1
                                        if (refreshTranslations(false)) {
                                                populate();
                                        }
                                }
                        }

                        Connections {
                                target: translationSelector
                                onAccepted: populate();
                        }

                        Component.onCompleted: {
                                content.addSecondaryText = _settings.translationMode == 1

                                populate();

                                if (pagePosition.isValid()) {
                                        scrollTo(pagePosition.sura, pagePosition.aya);
                                }
                        }
                }
        }

        Item {
                id: view
                property Item current: null

                Connections {
                        target: view.current
                        onLineVisibilityRequested: {
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
                }

                function pageNumberChanged() {
                        if (!current) {
                                return;
                        }

                        if (!_data.hasPage(_settings.pageNumber)) {
                                return;
                        }

                        if (current.page == _settings.pageNumber) {
                                flick.contentY = 0;
                                return;
                        }

                        if (_settings.pageNumber > index) {
                                index = _settings.pageNumber;
                                showNextItem();
                        }
                        else {
                                index = _settings.pageNumber;
                                showPreviousItem();
                        }
                }

                Component.onCompleted: {
                        addItem();
                        _settings.pageNumberChanged.connect(pageNumberChanged);
                }

                property int index: _settings.pageNumber

                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: toolBar.top

                function addItem() {
                        if (current) {
                                current.destroy(500);
                        }

                        current = quranPageDelegate.createObject(flick.contentItem);
                        current.page = view.index;
                }

                function showNextItem() {
                        nextAnimation.prev = current;
                        addItem();
                        nextAnimation.next = current;
                        current.x = -view.width;
                        nextAnimation.running = true;
                }

                function showPreviousItem() {
                        previousAnimation.prev = current;
                        addItem();
                        previousAnimation.next = current;
                        current.x = view.width;
                        previousAnimation.running = true;
                }

                ParallelAnimation {
                        id: nextAnimation
                        property alias prev: na.target
                        property alias next: nb.target
                        running: false
                        NumberAnimation {
                                id: na
                                duration: 250
                                property: "x"
                                from: 0
                                to: view.width
                        }
                        NumberAnimation {
                                id: nb
                                duration: 250
                                property: "x"
                                from: -view.width
                                to: 0
                        }

                        NumberAnimation {
                                duration: 250
                                target: flick
                                property: "contentY"
                                to: 0
                        }
                }

                ParallelAnimation {
                        id: previousAnimation
                        property alias prev: pa.target
                        property alias next: pb.target
                        running: false
                        NumberAnimation {
                                id: pa
                                duration: 250
                                property: "x"
                                from: 0
                                to: -view.width
                        }
                        NumberAnimation {
                                id: pb
                                duration: 250
                                property: "x"
                                from: view.width
                                to: 0
                        }

                        NumberAnimation {
                                duration: 250
                                target: flick
                                property: "contentY"
                                to: 0
                        }
                }

                Connections {
                        target: mouse
                        onSwipedRight: {
                                var newIndex = view.index + 1;
                                if (_data.hasPage(newIndex)) {
                                        view.index = newIndex;
                                        view.showNextItem();
                                        _settings.pageNumber = view.index;
                                }
                                else {
                                        lastPageReached.show();
                                }
                        }
                }

                Connections {
                        target: mouse
                        onSwipedLeft: {
                                var newIndex = view.index - 1;
                                if (_data.hasPage(newIndex)) {
                                        view.index = newIndex;
                                        view.showPreviousItem();
                                        _settings.pageNumber = view.index;
                                }
                                else {
                                        firstPageReached.show();
                                }
                        }
                }

                Item {
                        id: upper
                        anchors.top: parent.top
                        width: parent.width
                        height: Math.max(verse.height, part.height)
                        SuraList {
                                id: verse
                                anchors.left: parent.left
                                anchors.leftMargin: 10
                                suras: _data.surasForPage(_settings.pageNumber);
                        }

                        Label {
                                id: part
                                anchors.right: parent.right
                                text: _data.partName(_settings.pageNumber);
                        }
                }

                Flickable {
                        id: flick
                        anchors.top: upper.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        clip: true
                        contentHeight: view.current ? view.current.height : 0

                        flickableDirection: Flickable.VerticalFlick

                        interactive: !previousAnimation.running && !nextAnimation.running

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

                        MouseArea {
                                id: mouse
                                anchors.fill: parent

                                signal swipedLeft
                                signal swipedRight

                                property int xThreshold: 20
                                property int yThreshold: 60
                                property int x1: 0
                                property int x2: 0
                                property int y1: 0
                                property int y2: 0

                                acceptedButtons: Qt.LeftButton
                                enabled: flick.interactive

                                onPressAndHold: {
                                        if (!view.current) {
                                                return;
                                        }

                                        var b = view.current.bookmarkId(mouse.x, mouse.y);
                                        if (b == undefined) {
                                                return;
                                        }

                                        if (_bookmarks.isBookmarked(b)) {
                                                rmFavorite.bookmark = b;
                                                rmFavorite.text = view.current.textForPosition(mouse.x, mouse.y);
                                                rmFavorite.open();
                                        }
                                        else {
                                                addFavorite.bookmark = b;
                                                addFavorite.text = view.current.textForPosition(mouse.x, mouse.y);
                                                addFavorite.open();
                                        }
                                }

                                onClicked: {
                                        if (_settings.translationMode == 2) {
                                                // Hidden only:
                                                view.current.toggleSecondaryText(mouse.x, mouse.y);
                                        }
                                }

                                onPressed: {
                                        x1 = mouse.x; y1 = mouse.y;
                                }

                                onCanceled: {
                                        x2 = mouseX; y2 = mouseY;
                                        detectSwipe();
                                }

                                onReleased: {
                                        x2 = mouse.x; y2 = mouse.y;
                                        detectSwipe();
                                }

                                function detectSwipe() {
                                        var y = Math.abs(y1 - y2);
                                        var x = Math.abs(x1 - x2);
                                        if (y <= mouse.yThreshold && x >= mouse.xThreshold) {
                                                if (x1 > x2) {
                                                        swipedLeft();
                                                }
                                                else {
                                                        swipedRight();
                                                }
                                        }
                                }
                        }
                }
        }

        Menu {
                id: menu

                MenuLayout {
                        MenuItem { text: qsTr("Settings"); onClicked: { menu.close(); showPage("SettingsPage"); } }
//                        MenuItem { text: qsTr("Help") }
                        MenuItem { text: qsTr("About"); onClicked: { menu.close(); showPage("AboutPage"); } }
                        MenuItem { text: qsTr("Quit"); onClicked: Qt.quit(); }
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        id: layout

                        NumberLabel {
                                width: 60
                                height: 60
                                number: _settings.pageNumber
                                onClicked: showPage("IndexPage");
                        }

                        ToolButton {
                                icon: "imageviewer_favourite"
                                onClicked: {
                                        showPage(_bookmarks.empty ? "FavoritesPageEmpty" : "FavoritesPage");
                                }
                        }

                        ToolButton {
                                icon: "clock_starter_worldclock"
                                enabled: _settings.translationMode != 0

                                onEnabledChanged: layout.layout();

                                onClicked: _fsmon.available ? translationSelector.open() : massStorage.show();
                        }

                        ToolButton {
                                icon: "general_fullsize"
                                onClicked: _settings.fullScreen = !_settings.fullScreen;
                        }

                        ToolButton { icon: "keyboard_move_up"; onClicked: menu.open(); }
                }
        }
}
