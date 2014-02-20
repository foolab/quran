// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

SelectionDialog {
        id: dialog

        model: InstalledTranslationsModel {
                translations: _translations
        }

        delegate: SelectionDialogRow {
                selected: translationId == _translations.current
                onClicked: translationsManager.changeTranslation(translationId);
        }

        titleText: qsTr("Choose translation");
}
