// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

BackgroundItem {
        id: label
        property list<MenuAction> actions
        property alias content: row.children
        property alias contentHeight: row.height

        width: parent.width

        property Item _menu
        property Component _menuComponent: Component {
                ContextMenu {
                        Repeater {
                                model: label.actions
                                MenuItem {
                                        text: modelData.text
                                        visible: modelData.visible === true
                                        onClicked: modelData.clicked()
                                }
                        }
                }
        }

        onPressAndHold: {
                if (actions.length == 0) {
                    return
                }

                if (!_menu) {
                    _menu = _menuComponent.createObject(label)
                }

                _menu.show(label)
        }

        height: _menu && _menu.parent == label ? _menu.height + row.height : row.height

        Item {
                id: row
                height: Theme.itemSizeLarge
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: theme.marginSmall
                anchors.rightMargin: theme.marginSmall
        }
}
