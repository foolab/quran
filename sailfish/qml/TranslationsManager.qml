// -*- qml -*-
import QtQuick 2.0

QtObject {
        id: manager

        property bool enabled: false

        function changeTranslation(id) {
                if (!enabled) {
                        banner.show(qsTr("Failed to load the translation"))
                }

                if (!_translations.load(id)) {
                        banner.show(qsTr("Failed to load the translation"))
                }
        }

        function translationModeChanged() {
                if (_settings.translationMode != 0) {
                        if (!_translations.loadDefault()) {
                                if (_translations.installedCount == 0) {
                                        banner.show(qsTr("You need to download a translation first"))
                                }
                                else {
                                        banner.show(qsTr("Failed to load the translation"))
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
                _translations.refresh();

                translationModeChanged();
        }

        Component.onCompleted: {
                _settings.translationModeChanged.connect(translationModeChanged);
                reset();
        }
}
