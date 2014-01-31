// -*- qml -*-
import QtQuick 1.0

Row {
        id: row
        width: view.width
        height: text.height

        ToolButton {
                id: icon
                icon: theme.playPart
                enabled: settings.recitationMode != 0 && fsmon.available
                color: text.color
                onClicked: {
                        settings.pageNumber = quranData.pageNumberForPart(selectedIndex);
                        recitations.playPart(index);
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
