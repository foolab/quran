// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

ComboBox {
        property list<MenuAction> actions

        menu: ContextMenu {
                Repeater {
                        model: actions
                        MenuItem {
                                text: modelData.text
                        }
                }
        }
}
