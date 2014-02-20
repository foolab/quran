// -*- qml -*-
import QtQuick 1.0
import Quran 1.0

SelectionDialog {
        id: dialog

        model: RecitationModel {
                recitations: _recitations
        }

        delegate: SelectionDialogRow {
                selected: recitationId == _recitations.current;
                onClicked: recitationsManager.changeRecitation(recitationId);
        }

        titleText: qsTr("Choose recitation");
}
