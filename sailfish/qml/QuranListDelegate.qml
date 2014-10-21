// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranBackgroundItem {
    id: topItem

    width: ListView.view.width
    height: _menu && _menu.parent == topItem ? _menu.height + contentHeight : contentHeight

    property alias contentHeight: item.height
    property list<MenuAction> actions
    default property alias content: item.data
    property Item _menu

    property Component _menuComponent: Component {
        ContextMenu {
            Repeater {
                model: topItem.actions
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
            _menu = _menuComponent.createObject(topItem)
        }

        _menu.show(topItem)
    }

    Item {
        id: item

        anchors {
            top: parent.top
            left: parent.left
            leftMargin: quranTheme.marginMedium
            right: parent.right
            rightMargin: quranTheme.marginMedium
        }
    }
}
