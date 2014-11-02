// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Item {
    id: item
    property list<MenuAction> actions
    property Item _menu
    property Item container
    property int menuHeight: _menu && _menu.parent == container ? _menu.height : 0

    property Component _menuComponent: Component {
        ContextMenu {
            Repeater {
                model: item.actions
                MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onClicked: modelData.clicked()
                }
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

        _menu.show(container)
    }
}
