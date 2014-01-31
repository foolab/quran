// -*- qml -*-
import QtQuick 1.0

QtObject {
        id: manager

        property bool enabled: false

        function changeRecitation(id) {
                if (!enabled) {
                        recitationError.show();
                }

                if (!recitations.load(id)) {
                        recitationError.show();
                }
        }

        function recitationModeChanged() {
                if (settings.recitationMode != 0) {
                        if (!recitations.loadDefault()) {
                                if (recitations.installed.length == 0) {
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
                        recitations.unload();
                        enabled = false;
                }
        }

        function reset() {
                if (!fsmon.available) {
                        massStorage.show();
                        enabled = false;
                        return;
                }

                recitations.refresh();
                recitationModeChanged();
        }

        Component.onCompleted: {
                fsmon.availabilityChanged.connect(reset);
                settings.recitationModeChanged.connect(recitationModeChanged);
                reset();
        }
}
