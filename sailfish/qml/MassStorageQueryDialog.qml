// -*- qml -*-
import QtQuick 2.0

QueryDialog {
        id: dialog
        Connections {
                target: _fsmon
                onAvailableChanged: {
                        if (!_fsmon.available) {
                                dialog.close();
                                dialog.rejected();
                        }
                }
        }
}
