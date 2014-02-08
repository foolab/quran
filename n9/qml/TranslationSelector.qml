// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

SelectionDialog {
        id: dialog

        model: InstalledTranslationsModel {
                translations: _translations
        }

        delegate: TranslationLabel {
                width: dialog.width
                tid: translationId
                property bool selected: tid == _translations.current
                color: pressed ? _colors.pressedColor : selected ? _colors.selectionBackgroundColor : _colors.backgroundColor
                textColor: pressed ? _colors.pressedTextColor : selected ? _colors.selectionTextColor : _colors.textColor
                onClicked: {
                        translationsManager.changeTranslation(translationId);
                        dialog.accept();
                }
        }

        titleText: qsTr("Choose translation");
}
