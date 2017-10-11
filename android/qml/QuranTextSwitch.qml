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

Switch {
    id: control
    checkable: true
    property bool automaticCheck: true
    // We want the indicator on the right
    LayoutMirroring.enabled: true

    anchors {
        left: parent.left
        right: parent.right
        leftMargin: quranTheme.marginSmall
        rightMargin: quranTheme.marginSmall
    }

    height: quranTheme.itemSizeSmall

    contentItem: QuranLabel {
        id: label
        leftPadding: 0
        rightPadding: control.indicator.width + control.spacing
        text: control.text
        verticalAlignment: Text.AlignVCenter
        color: quranTheme.primaryColor
    }

    MouseArea {
        anchors.fill: parent
        enabled: !parent.automaticCheck
        onClicked: parent.clicked()
    }
}
