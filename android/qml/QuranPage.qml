// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
        id: page
        clip: pageStack && pageStack.depth > 1

        property bool __toolBarVisible: toolBarItem.children.length > 0
        property string __pageTitle
        property QuranPageMenu menu

        // All children of the page created by our app will be children of this item
        // And all of our children (items) will be children of the real Page content item!
        default property alias _content: contentItem.data

        property alias toolBar: toolBarItem.children
//        allowedOrientations: settings.orientation == 1 ? Orientation.Portrait | Orientation.PortraitInverted : settings.orientation == 2 ? Orientation.Landscape | Orientation.LandscapeInverted : Orientation.All

        Rectangle {
                anchors.fill: parent
                color: quranTheme.backgroundColor
        }

        function combine(page, props1, props2) {
                if (props1 != undefined) {
                    props2["properties"] = props1
                }

                props2["item"] = page
                props2["destroyOnPop"] = true

                return props2
        }

        function replaceAnimated(page, props) {
                return pageStack.push(combine(page, props, {immediate: false, replace: true}))
        }

        function replaceImmediate(page, props) {
                return pageStack.push(combine(page, props, {immediate: true, replace: true}))
        }

        function pushAnimated(page, props) {
                return pageStack.push(combine(page, props, {immediate: false}))
        }

        function pushImmediate(page, props) {
                return pageStack.push(combine(page, props, {immediate: true}))
        }

        function popPage() {
                pageStack.pop()
        }

        function popToFirst() {
                pageStack.pop(null)
        }

        Item {
                id: contentItem
                width: parent.width
                anchors.top: parent.top
                anchors.bottom: bar.top
        }

        ToolBar {
                id: bar
                width: parent.width
                height: toolBarItem.children.length > 0 ? quranTheme.toolBarHeight : 0
                anchors.bottom: parent.bottom
                visible: height > 0

                Rectangle {
                        anchors.fill: parent
                        color: quranTheme.backgroundColor
                        height: parent.height

                        Row {
                                id: toolBarItem
                                height: parent.height
                                anchors.top: parent.top
                                anchors.horizontalCenter: parent.horizontalCenter
                                spacing: quranTheme.spacing
                        }

                        Item {
                                width: quranTheme.separator
                                height: parent.height
                                anchors.left: toolBarItem.right
                        }

                        MenuButton {
                                anchors.left: toolBarItem.right
                                anchors.right: parent.right
                                menu: page.menu
                        }
                }
        }
}
//TODO:
