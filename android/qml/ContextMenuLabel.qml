// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

QuranBackgroundItem {
        id: label
        property list<MenuAction> actions
        property alias content: row.children
        property alias contentHeight: row.height

        width: parent ? parent.width : quranTheme.itemSizeLarge
        height: row.height

        property QtObject _menu
        property Component _menuComponent: Component {
                Menu {
                        id: menu
                        Instantiator {
                                model: label.actions
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

        onPressAndHold: {
                if (actions.length == 0) {
                    return
                }

                if (!_menu) {
                    _menu = _menuComponent.createObject(label)
                }

                _menu.popup()
        }

        Item {
                id: row
                height: quranTheme.itemSizeSmall
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: quranTheme.marginSmall
                anchors.rightMargin: quranTheme.marginSmall
        }
}
