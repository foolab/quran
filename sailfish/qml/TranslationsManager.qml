// -*- qml -*-
import QtQuick 2.0

QtObject {
        id: manager

        property bool enabled: false

        function changeTranslation(id) {
                if (!enabled) {
                        translationError.show();
                }

                if (!_translations.load(id)) {
                        translationError.show();
                }
        }

        function translationModeChanged() {
                if (_settings.translationMode != 0) {
                        if (!_translations.loadDefault()) {
                                if (_translations.installed.length == 0) {
                                        noTranslations.show();
                                }
                                else {
                                        translationError.show();
                                }

                                enabled = false;
                                return;
                        }

                        enabled = false; // HACK
                        enabled = true;
                        return;
                }
                else {
                        _translations.unload();
                       enabled = false;
                }
        }

        function reset() {
                if (!_fsmon.available) {
                        massStorage.show();
                        enabled = false;
                        return;
                }

                _translations.refresh();

                translationModeChanged();
        }

        Component.onCompleted: {
                _fsmon.availabilityChanged.connect(reset);
                _settings.translationModeChanged.connect(translationModeChanged);
                reset();
        }
}
