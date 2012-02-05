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
                onClicked: {
                        _settings.pageNumber = _data.pageNumberForPart(selectedIndex);
                        _recitations.playPart(index);
                        accept();
                }

                height: text.height
        }

        SelectionDialogRow {
                id: text
                width: parent.width - icon.width
                textHorizontalAlignment: Text.AlignRight
        }

}
