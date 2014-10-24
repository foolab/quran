// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Item {
    id: item
    property list<MenuAction> actions
    property Item _menu
    property Item container
    property int menuHeight: 0

    property Component _menuComponent: Component {
        ContextMenu {
            id: menu
            Repeater {
                model: item.actions
                MenuItem {
                    text: modelData.text
                    visible: modelData.visible === true
                    onClicked: { menu.close(); modelData.clicked() }
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

        if (!_menu) {
            _menu = _menuComponent.createObject(item)
        }

        _menu.open()
    }
}
