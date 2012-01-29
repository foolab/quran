// -*- qml -*-
import QtQuick 1.0

SelectionDialog {
        id: partDialog
        titleText: qsTr("Choose a part");

        delegate: PartSelectionDialogRow {}

        model: ListModel {
                Component.onCompleted: {
                        for (var x = 0; x < 30; x++) {
                                append({"name": _data.partName(x) });
                        }
                }
        }

        onStateChanged: {
                if (isOpen()) {
                        selectedIndex = _data.partNumberForPage(_settings.pageNumber);
                }
        }

        onAccepted: {
                _settings.pageNumber = _data.pageNumberForPart(selectedIndex);
                pageStack.pop();
        }
}
