// -*- qml -*-
import QtQuick 1.1
import com.nokia.meego 1.0

Page {
        id: page
        anchors.fill: parent
        property alias toolBar: toolBarItem.children
        property Item menu

        orientationLock: settings.orientation == 1 ? PageOrientation.LockPortrait : settings.orientation == 2 ? PageOrientation.LockLandscape : PageOrientation.Automatic
        tools: toolBarContainer

        function replaceAnimated(page, props) {
                return pageStack.replace(page, props, false)
        }

        function replaceImmediate(page, props) {
                return pageStack.replace(page, props, true)
        }

        function pushAnimated(page, props) {
                return pageStack.push(page, props, false)
        }

        function pushImmediate(page, props) {
                return pageStack.push(page, props, true)
        }

        function popPage() {
                pageStack.pop()
        }

        function popToFirst() {
                pageStack.pop(null)
        }

        ToolBarLayout {
                id: toolBarContainer

                ToolIcon {
                        id: backIcon;
                        visible: pageStack && pageStack.depth > 1 ? true : false
                        iconId: "toolbar-back"
                        onClicked: pageStack.pop()
                }

                ToolButtonRow {
                        id: toolBarItem
                }

                ToolIcon {
                        id : menuItem;
                        iconId: "toolbar-view-menu"
                        visible: page.menu != null
                        onClicked: page.menu.open()
                }
        }
}
