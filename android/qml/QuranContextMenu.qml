// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
    id: item
    property list<MenuAction> actions
    property QtObject _menu
    property Item container
    property int menuHeight: 0

    property Component _menuComponent: Component {
        Menu {
            id: menu
            Instantiator {
                model: topItem.actions
                delegate: MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onTriggered: modelData.clicked()
                }

                onObjectAdded: menu.insertItem(index, object)
                onObjectRemoved: menu.removeItem(object)
            }
        }
    }

    function clicked() {
        // Nothing
    }

    function pressAndHold() {
        if (actions.length == 0) {
            return
        }

        var enabledActions = 0
        for (var x = 0; x < actions.length; x++) {
            if (actions[x].visible == true) {
                enabledActions++
                break
            }
        }

        if (enabledActions == 0) {
            return
        }

        if (!_menu) {
            _menu = _menuComponent.createObject(item)
        }

        _menu.popup()
    }
}
