// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

ApplicationWindow {
        id: root

        cover: Component {
                CoverBackground {
                    CoverPlaceholder {
                        icon.source: "qrc:/logo.png"
                    }
                }
        }

        Settings {
                id: settings
        }

        Downloader {
                id: _downloader
        }

        DataProvider {
                id: _data
        }

        Bookmarks {
                id: _bookmarks
                settings: settings
        }

        Translations {
                id: _translations
                settings: settings
                downloader: _downloader
                data: _data
        }

        PhoneFlipControl {
                id: flipControl
                active: settings.flipToStopRecitation && _recitations.isPlaying
                onFlipped: _recitations.stop()
        }

        Recitations {
                id: _recitations
                settings: settings
                data: _data
                downloader: _downloader
                onError: banner.show(qsTr("Failed to play recitation"))
        }

        QuranTheme {
                id: theme
                inNightMode: settings.nightMode
        }

        TranslationsManager {
                id: translationsManager
        }

        RecitationsManager {
                id: recitationsManager
        }

        initialPage: Component {
                MainPage {}
        }

        Component.onCompleted: _data.setTextType(settings.textType)

        QtObject {
                id: pagePosition

                property int sura: -1
                property int aya: -1

                signal changed

                function isValid() {
                        return sura != -1 && aya != -1;
                }

                function reset() {
                        sura = -1;
                        aya = -1;
                }

                function setPosition(sura, aya) {
                        pagePosition.sura = sura
                        pagePosition.aya = aya
                        settings.pageNumber = _data.pageNumberForSuraAndAya(sura, aya)
                        changed()
                }

//                onSuraChanged: console.log("Sura " + sura);
//                onAyaChanged: console.log("Aya " + aya);
//                onYChanged: console.log("Y " + y);
        }

        Connections {
                target: settings
                onTextTypeChanged: {
                        var type = _data.textType();
                        if (!_data.setTextType(settings.textType)) {
                                settings.textType = type;
                        }
                }
        }

        Rectangle {
                id: banner
                width: parent.width
                height: Theme.itemSizeSmall
                color: Theme.highlightColor
                anchors.top: parent.top
                opacity: 0
                visible: opacity > 0

                Behavior on opacity {
                        NumberAnimation { duration: 200 }
                }

                function show(text) {
                        timer.restart()
                        label.text = text
                        banner.opacity = 1
                }

                Timer {
                        id: timer
                        interval: 3000
                        running: banner.opacity == 1
                        onTriggered: banner.opacity = 0
                }

                Label {
                        id: label
                        anchors.fill: parent
                        anchors.leftMargin: theme.largeMedium
                        color: Theme.primaryColor
                        truncationMode: TruncationMode.Fade
                        verticalAlignment: Text.AlignVCenter
                        font.family: Theme.fontFamily
                }
        }

/*
        InfoBanner {
                id: translations
        }

        Connections {
                target: _recitations
                onError: { playerError.text = msg; playerError.show(); }
        }

        Connections {
                target: _translations
                onInstalled: {
                        translations.text = qsTr("Installed ") + _translations.translationName(id);
                        translations.show();
                }

                onFailed: {
                        translations.text = qsTr("Failed to download ") + _translations.translationName(id);
                        translations.show();
                }

                onRemoved: {
                        translations.text = qsTr("Removed ") + _translations.translationName(id);
                        translations.show();
                }
        }
*/
}
