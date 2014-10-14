// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
        // All children of the page created by our app will be children of this item
        // And all of our children (items) will be children of the real Page content item!
        default property alias _content: contentItem.data
        property Item menu
        property alias toolBar: toolBarItem.children
        allowedOrientations: settings.orientation == 1 ? Orientation.Portrait | Orientation.PortraitInverted : settings.orientation == 2 ? Orientation.Landscape | Orientation.LandscapeInverted : Orientation.All

        function replaceAnimated(page, props) {
                return pageStack.replace(page, props, PageStackAction.Animated)
        }

        function replaceImmediate(page, props) {
                return pageStack.replace(page, props, PageStackAction.Immediate)
        }

        function pushAnimated(page, props) {
                return pageStack.push(page, props, PageStackAction.Animated)
        }

        function pushImmediate(page, props) {
                return pageStack.push(page, props, PageStackAction.Immediate)
        }

        function popPage() {
                pageStack.pop()
        }

        function popToFirst() {
                var p = pageStack.previousPage()
                var page = p
                while (page != null) {
                    page = pageStack.previousPage(page)
                    if (page != null) {
                        p = page
                    }
                }

                pageStack.pop(p)
        }

        Item {
                id: contentItem
                width: parent.width
                anchors.top: parent.top
                anchors.bottom: mouseGrabber.top
        }

        MouseArea {
                id: mouseGrabber
                width: parent.width
                anchors.bottom: parent.bottom
                height: toolBar.length > 0 ? Theme.itemSizeSmall : 0

                Rectangle {
                        anchors.fill: parent
                        color: quranTheme.backgroundColor
                }

                Row {
                        id: toolBarItem
                        height: parent.height
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                }
        }
}
