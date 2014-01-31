// -*- qml -*-
import QtQuick 1.0

SelectionDialog {
        id: partDialog
        titleText: qsTr("Choose a part");

        delegate: PartSelectionDialogRow {}

        model: ListModel {
                Component.onCompleted: {
                        for (var x = 0; x < 30; x++) {
                                append({"name": quranData.partName(x) });
                        }
                }
        }

        onStateChanged: {
                if (isOpen()) {
                        selectedIndex = quranData.partNumberForPage(settings.pageNumber);
                }
        }

        onAccepted: {
                settings.pageNumber = quranData.pageNumberForPart(selectedIndex);
                pageStack.pop();
        }
}
