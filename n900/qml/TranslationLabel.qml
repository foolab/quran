// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

DownloadLabel {
        id: label

        property int tid: -1

        property alias status: translation.status

        property bool showCategory: false

        showProgress: translation.status == Translation.Downloading
        showError: translation.status == Translation.Error

        progress: translation.downloadProgress
        showInstalled: translation.status == Translation.Installed
        errorText: translation.error

        function resetText() {
                if (tid < 0) {
                        return;
                }

                var t;
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
        }
}
