// -*- qml -*-
import QtQuick 2.0

QtObject {
        id: manager

        property bool enabled: false

        function changeRecitation(id) {
                if (!enabled) {
                        banner.show(qsTr("Failed to load the recitation"))
                }

                if (!_recitations.load(id)) {
                        banner.show(qsTr("Failed to load the recitation"))
                }
        }

        function recitationModeChanged() {
                if (_settings.recitationMode != 0) {
                        if (!_recitations.loadDefault()) {
                                if (_recitations.installedCount == 0) {
                                        banner.show(qsTr("You need to install a recitation first"))
                                }
                                else {
                                        banner.show(qsTr("Failed to load the recitation"))
                                }

                                enabled = false;
                                return;
                        }

                        enabled = false; // HACK
                        enabled = true;
                        return;
                }
                else {
                        _recitations.unload();
                        enabled = false;
                }
        }

        function reset() {
                _recitations.refresh();
                recitationModeChanged();
        }

        Component.onCompleted: {
                _settings.recitationModeChanged.connect(recitationModeChanged);
                reset();
        }
}
