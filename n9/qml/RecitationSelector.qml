// -*- qml -*-
import QtQuick 1.0

SelectionDialog {
        id: dialog

        model: ListModel {
                id: model

                function currentChanged() {
                        var len = recitations.installed.length;

                        for (var x = 0; x < len; x++) {
                                if (recitations.current == get(x).rid) {
                                        selectedIndex = x;
                                        return;
                                }
                        }
                }

                function populate() {
                        clear();

                        var len = recitations.installed.length;

                        for (var x = 0; x < len; x++) {
                                var rid = recitations.installed[x];
                                var name = recitations.recitationName(rid);
                                append({"rid": rid, "name": name});
                        }

                        currentChanged();
                }
        }

        titleText: qsTr("Choose recitation");

        Connections {
                target: recitations
                onInstalledChanged: model.populate();
                onCurrentChanged: model.currentChanged();
        }

        Component.onCompleted: model.populate();

        onAccepted: {
                var rid = model.get(selectedIndex).rid;
                recitationsManager.changeRecitation(rid);
        }
}
