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

import QtQuick 2.2
import QtQuick.Controls 2.2

Item {
    id: page

    property string __pageTitle
    property QuranPageMenu menu
    property list<Item> toolBar

    Rectangle {
        anchors.fill: parent
        color: quranTheme.colors.background
    }

    function combine(page, props1, props2) {
        if (props1 != undefined) {
            props2["properties"] = props1
        }

        props2["item"] = page
        props2["destroyOnPop"] = true

        return props2
    }

    function pushAnimated(page, props) {
        return pageStack.push(page, props, StackView.PushTransition)
    }

    function pushImmediate(page, props) {
        return pageStack.push(page, props, StackView.Immediate)
    }

    function popPage() {
        pageStack.pop()
    }

    function popToFirst() {
        pageStack.pop(null)
    }
}
