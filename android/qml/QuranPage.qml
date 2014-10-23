// -*- qml -*-
import QtQuick 2.2
import QtQuick.Controls 1.2

Item {
        id: page

        property string __pageTitle
        property QuranPageMenu menu
        property list<Item> toolBar

// TODO:
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
}
