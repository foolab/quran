// -*- qml -*-
import QtQuick 1.0

SelectionDialog {
        id: dialog

        model: ListModel {
                id: model

                function currentChanged() {
                        var len = translations.installed.length;

                        for (var x = 0; x < len; x++) {
                                if (translations.current == get(x).tid) {
                                        selectedIndex = x;
                                        return;
                                }
                        }
                }

                function populate() {
                        clear();

                        var len = translations.installed.length;

                        for (var x = 0; x < len; x++) {
                                var tid = translations.installed[x];
                                var name = translations.translationName(tid);
                                append({"tid": tid, "name": name});
                        }

                        currentChanged();
                }
        }

        titleText: qsTr("Choose translation");

        Connections {
                target: translations
                onInstalledChanged: model.populate();
                onCurrentChanged: model.currentChanged();
        }

        Component.onCompleted: model.populate();

        onAccepted: {
                var tid = model.get(selectedIndex).tid;
                translationsManager.changeTranslation(tid);
        }
}
