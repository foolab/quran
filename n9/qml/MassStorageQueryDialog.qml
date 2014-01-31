// -*- qml -*-
import QtQuick 1.0

QueryDialog {
        id: dialog
        Connections {
                target: fsmon
                onAvailableChanged: {
                        if (!fsmon.available) {
                                dialog.close();
                                dialog.rejected();
                        }
                }
        }
}
