// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

DownloadLabel {
        id: label

        property int tid: -1
        property alias status: translation.status

        onStatusChanged: {
                if (status == Translation.Error) {
                        banner.show(translation.error)
                }
        }

        property bool showCategory: false

        showProgress: translation.status == Translation.Downloading
        progress: translation.downloadProgress
        showInstalled: translation.status == Translation.Installed

        RemorseItem { id: remorse }

        menuComponent: Component {
                ContextMenu {
                        MenuItem {
                                text: qsTr("Download")
                                onClicked: _translations.startDownload(tid)
                                visible: status == Translation.None || status == Translation.Error
                        }

                        MenuItem {
                                text: qsTr("Stop")
                                onClicked: _translations.stopDownload(tid)
                                visible: status == Translation.Downloading
                        }

                        MenuItem {
                                text: qsTr("Remove")
                                onClicked: remorse.execute(label, "Removing", function() { _translations.removeTranslation(tid) } )
                                visible: status == Translation.Installed
                        }
                }
        }

        function resetText() {
                if (tid < 0) {
                        return;
                }

                var t = "";
                if (showCategory) {
                        t = _translations.categoryNameForTranslation(tid) + " - ";
                }

                t += _translations.translationName(tid);

                text = t;
        }

        onTidChanged: resetText();
        onShowCategoryChanged: resetText();
        Component.onCompleted: resetText();

        Translation {
                id: translation
                tid: label.tid
                translations: _translations
                Component.onCompleted: init();
        }
}
