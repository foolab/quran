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
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Button {
    height: quranTheme.itemSizeSmall
    style: Component {
        ButtonStyle {
            id: buttonStyle
            readonly property int renderType: Text.NativeRendering

            background: Rectangle {
                anchors.fill: parent
                color: control.pressed ? quranTheme.inNightMode ? "#191919" : "#e5e5e5" : quranTheme.backgroundColor
                border.width: 2
                border.color: control.pressed ? quranTheme.primaryColor : quranTheme.highlightColor
                radius: border.width * 2
                implicitWidth: quranTheme.itemSizeLarge * 2
            }


            label: QuranLabel {
                text: control.text
                color: quranTheme.primaryColor
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                renderType: buttonStyle.renderType
                anchors {
                    top: parent.top
                    bottom: parent.bottom
                    left: parent.left
                    leftMargin: quranTheme.marginMedium
                    right: parent.right
                    rightMargin: quranTheme.marginMedium
                }
            }
        }
    }
}
