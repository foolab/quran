// -*- qml -*-
import QtQuick 2.0

QtObject {
        id: manager

        property bool enabled: false

        function changeRecitation(id) {
                if (!enabled) {
                        recitationError.show();
                }

                if (!_recitations.load(id)) {
                        recitationError.show();
                }
        }

        function recitationModeChanged() {
                if (_settings.recitationMode != 0) {
                        if (!_recitations.loadDefault()) {
                                if (_recitations.installed.length == 0) {
                                        noRecitations.show();
                                }
                                else {
                                        recitationError.show();
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
                if (!_fsmon.available) {
                        massStorage.show();
                        enabled = false;
                        return;
                }

                _recitations.refresh();
                recitationModeChanged();
        }

        Component.onCompleted: {
                _fsmon.availabilityChanged.connect(reset);
                _settings.recitationModeChanged.connect(recitationModeChanged);
                reset();
        }
}
