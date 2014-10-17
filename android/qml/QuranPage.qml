// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
        id: page

        // All children of the page created by our app will be children of this item
        // And all of our children (items) will be children of the real Page content item!
        default property alias _content: contentItem.data
        property QuranPageMenu menu
        property alias toolBar: toolBarItem.children
//        allowedOrientations: settings.orientation == 1 ? Orientation.Portrait | Orientation.PortraitInverted : settings.orientation == 2 ? Orientation.Landscape | Orientation.LandscapeInverted : Orientation.All

        function combine(page, props1, props2) {
                props2["item"] = page
                props2["destroyOnPop"] = true
                // TODO: menu -> settings -> manage translations -> add -> tap on language
                // and this fails
                // TODO: also index and tap on verse selection => fail
                for (x in props1) {
                        props2[x] = props1[x]
                }

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
/*
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
*/

        Item {
                id: contentItem
                width: parent.width
                anchors.top: parent.top
                anchors.bottom: mouseGrabber.top
        }

        ToolBar {
                id: mouseGrabber
                width: parent.width
                anchors.bottom: parent.bottom

//                Rectangle {
//                        anchors.fill: parent
//                        color: quranTheme.backgroundColor
//                }

                ToolButton {
                        id: backIcon;
                        visible: pageStack && pageStack.depth > 1 ? true : false
//                        iconId: "toolbar-back"
                        onClicked: popPage()
                }

                Row {
                        id: toolBarItem
                        height: parent.height
                        anchors.top: parent.top
                        anchors.horizontalCenter: parent.horizontalCenter
                }

                ToolButton {
                        visible: page.menu != null
                        onClicked: page.menu.popup()
                }

        }
}
//TODO:
