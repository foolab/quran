// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

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
