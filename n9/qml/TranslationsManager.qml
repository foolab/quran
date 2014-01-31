// -*- qml -*-
import QtQuick 1.0

QtObject {
        id: manager

        property bool enabled: false

        function changeTranslation(id) {
                if (!enabled) {
                        translationError.show();
                }

                if (!translations.load(id)) {
                        translationError.show();
                }
        }

        function translationModeChanged() {
                if (settings.translationMode != 0) {
                        if (!translations.loadDefault()) {
                                if (translations.installed.length == 0) {
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
                        translations.unload();
                       enabled = false;
                }
        }

        function reset() {
                if (!fsmon.available) {
                        massStorage.show();
                        enabled = false;
                        return;
                }

                translations.refresh();

                translationModeChanged();
        }

        Component.onCompleted: {
                fsmon.availabilityChanged.connect(reset);
                settings.translationModeChanged.connect(translationModeChanged);
                reset();
        }
}
