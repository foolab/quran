// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

ComboBox {
        property list<MenuAction> actions
        property string label
/*
        menu: ContextMenu {
                Repeater {
                        model: actions
                        MenuItem {
                                text: modelData.text
                        }
                }
        }
*/
}
//TODO:
