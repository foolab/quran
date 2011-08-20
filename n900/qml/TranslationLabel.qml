// -*- qml-mode -*-
import QtQuick 1.0
import Translations 1.0

DownloadLabel {
        id: label

        property int tid: -1

        property alias status: translation.status

        showProgress: translation.status == Translation.Downloading
        showError: translation.status == Translation.Error

        progress: translation.downloadProgress
        showRm: translation.status != Translation.None
        text: tid >= 0 ? _translations.translationName(tid) : "";
        errorText: translation.error

        Translation {
                id: translation
                tid: label.tid
                translations: _translations
        }
}
