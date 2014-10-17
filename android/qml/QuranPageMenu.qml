// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Menu {
        id: menu
        property list<MenuAction> actions
        property variant view

        Instantiator  {
                model: actions

                delegate: MenuItem {
                        text: modelData.text
                        onTriggered: modelData.clicked()
                }

                 onObjectAdded: menu.insertItem(index, object)
                 onObjectRemoved: menu.removeItem(object)
        }
}
//TODO:
