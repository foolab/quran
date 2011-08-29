// -*- qml-mode -*-
import QtQuick 1.0

Page {

        // TODO: reset flick when we enter full screen (when we are at the end of the page).
        id: quranPage
        tools: toolBar

        NavigationBar {
                id: navBar
                z: 1500
                y: view.y
        }

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

        function showAddToFavoritesMenu(text, chapter, verse) {
                addFavorite.bookmark = _bookmarks.serialize(chapter, verse);
                addFavorite.text = text;
                addFavorite.open();
        }

        function showRemoveFromFavoritesMenu(text, chapter, verse) {
                rmFavorite.bookmark = _bookmarks.serialize(chapter, verse);;
                rmFavorite.text = text;
                rmFavorite.open();
        }

        Component {
                id: quranPageDelegate

                QuranView {
                        id: content
                        onMovementStarted: navBar.show();
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
                        anchors.rightMargin: 10
                        text: _data.partName(_settings.pageNumber);
                }
        }

        Item {
                id: view
                property Item page: null

                anchors.top: upper.bottom
                anchors.bottom: toolBar.top
                width: parent.width

                function pageNumberChanged() {
                        if (page && page.page == _settings.pageNumber) {
                                return;
                        }

                        var newPage = quranPageDelegate.createObject(view);
                        newPage.page = _settings.pageNumber;
                        newPage.showAddToFavoritesMenu.connect(showAddToFavoritesMenu);
                        newPage.showRemoveFromFavoritesMenu.connect(showRemoveFromFavoritesMenu);

                        if (page) {
                                page.hide();
                        }

                        page = newPage;
                        page.show();
                }

                Component.onCompleted: {
                        _settings.pageNumberChanged.connect(pageNumberChanged);
                        pageNumberChanged();
                }

                Connections {
                        target: mouse
                        onSwipedRight: navBar.show();
                        onSwipedLeft: navBar.show();
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
                        enabled: view.page ? view.page.interactive: false

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
                }
        }

        Connections {
                target: navBar
                onNextClicked: {
                        navBar.show();
                        var newIndex = _settings.pageNumber + 1;
                        if (_data.hasPage(newIndex)) {
                                _settings.pageNumber = newIndex;
                        }
                        else {
                                lastPageReached.show();
                        }
                }

                onPreviousClicked: {
                        navBar.show();
                        var newIndex = _settings.pageNumber - 1;
                        if (_data.hasPage(newIndex)) {
                                _settings.pageNumber = newIndex;
                        }
                        else {
                                firstPageReached.show();
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
                                icon: theme.favorites
                                onClicked: {
                                        showPage(_bookmarks.empty ? "FavoritesPageEmpty" : "FavoritesPage");
                                }
                        }

                        ToolButton {
                                icon: theme.translations
                                enabled: _settings.translationMode != 0

                                onEnabledChanged: layout.layout();

                                onClicked: _fsmon.available ? translationSelector.open() : massStorage.show();
                        }

                        ToolButton {
                                icon: _settings.fullScreen ? theme.normalView : theme.fullScreenView
                                onClicked: _settings.fullScreen = !_settings.fullScreen;
                        }

                        ToolButton { icon: theme.menuIcon; onClicked: menu.open(); }
                }
        }
}
