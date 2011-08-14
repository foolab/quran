// -*- qml-mode -*-
import QtQuick 1.0

// TODO: Depends on qtquickcompat

PageStackWindow {
        id: root

        Item {
                id: pagePosition
                property int sura
                property int aya
//                property int y
        }

        Connections {
                target: _settings
                onTextTypeChanged: {
                        var type = _data.textType();
                        if (!_data.setTextType(_settings.textType)) {
                                textError.show();
                                _settings.textType = type;
                        }
                }
        }

        InfoBanner {
                id: favoriteAdded
                text: qsTr("Favorite added");

                Component.onCompleted: {
                        _bookmarks.bookmarkAdded.connect(show);
                }
        }

        InfoBanner {
                id: favoriteRemoved
                text: qsTr("Favorite removed");

                Component.onCompleted: {
                        _bookmarks.bookmarkRemoved.connect(show);
                }
        }

        InfoBanner {
                id: favoritesCleared
                text: qsTr("Favorites cleared");

                Component.onCompleted: {
                        _bookmarks.cleared.connect(show);
                }
        }

        InfoBanner {
                id: maxFontSize
                text: qsTr("Maximum font size reached");
        }

        InfoBanner {
                id: minFontSize
                text: qsTr("Minimum font size reached");
        }

        InfoBanner {
                id: fontError
                text: qsTr("Failed to load application font");
        }

        InfoBanner {
                id: textError
                text: qsTr("Failed to load the text");
        }

        InfoBanner {
                id: firstPageReached
                text: qsTr("This is the first page");
        }

        InfoBanner {
                id: lastPageReached
                text: qsTr("This is the last page");
        }

        function showPage(name, point, now) {
                pageStack.push(name, point, now);
        }

        Component.onCompleted: {
                if (!_settings.fontLoaded) {
                        fontError.show();
                }

                if (_data.setTextType(_settings.textType)) {
                        pageStack.push("QuranPage", Qt.point(0, 0), true);
                }
                else {
                        pageStack.push("ErrorPage", Qt.point(0, 0), true);
                }
        }
}
