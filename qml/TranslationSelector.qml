// -*- qml -*-
import QtQuick 1.0

SelectionDialog {
        id: dialog

        model: ListModel {
                id: model

                function currentChanged() {
                        var len = _translations.installed.length;

                        for (var x = 0; x < len; x++) {
                                if (_translations.current == get(x).tid) {
                                        selectedIndex = x;
                                        return;
                                }
                        }
                }

                function populate() {
                        clear();

                        var len = _translations.installed.length;

                        for (var x = 0; x < len; x++) {
                                var tid = _translations.installed[x];
                                var name = _translations.translationName(tid);
                                append({"tid": tid, "name": name});
                        }

                        currentChanged();
                }
        }

        titleText: qsTr("Choose translation");

        Connections {
                target: _translations
                onInstalledChanged: model.populate();
                onCurrentChanged: model.currentChanged();
        }

        Component.onCompleted: model.populate();

        onAccepted: {
                var tid = model.get(selectedIndex).tid;
                translationsManager.changeTranslation(tid);
        }
}
