// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Menu {
    id: menu
    property list<MenuAction> actions
    property variant view

    Instantiator  {
        function reverse(m) {
            var l = m.length
            var out = []

            for (var x = 0; x < l; x++) {
                out[x] = m[l - x - 1]
            }

            return out
        }

        model: reverse(actions)

        delegate: MenuItem {
            text: modelData.text
            onTriggered: modelData.clicked()
        }

        onObjectAdded: menu.insertItem(index, object)
        onObjectRemoved: menu.removeItem(object)
    }
}
