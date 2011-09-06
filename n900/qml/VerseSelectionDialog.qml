// -*- qml -*-
import QtQuick 1.0

SelectionDialog {
        id: dialog

        property int chapter: -1

        selectedIndex: 0

        model: ListModel {
                id: model

                function populate() {
                        clear();

                        if (dialog.chapter == -1) {
                                return;
                        }

                        var len = _data.suraSize(dialog.chapter);

                        for (var x = 0; x < len; x++) {
                                append({"verse": x, "name": x + 1});
                        }
                }
        }

        onChapterChanged: {
                model.populate();
                selectedIndex = 0;
        }

        titleText: qsTr("Choose a verse");

        onAccepted: {
                pagePosition.setPosition(dialog.chapter, dialog.selectedIndex);
                pageStack.pop();
        }
}
