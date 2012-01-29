// -*- qml -*-
import QtQuick 1.0

Row {
        id: row
        width: view.width
        height: text.height

        ToolButton {
                id: icon
                icon: theme.playPart
                enabled: _settings.recitationMode != 0 && _fsmon.available
                color: text.color
                onClicked: _recitations.playPart(index);
                height: text.height
        }

        SelectionDialogRow {
                id: text
                width: parent.width - icon.width
        }

}
