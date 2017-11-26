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
import Quran 1.0

QuranPage {
    id: quranPage

    Rectangle {
        anchors.fill: parent
        color: quranTheme.quranColors.background
    }

    QuranListView {
        id: mainView
        clip: true
        anchors {
            top: parent.top
            left: parent.left
            right: parent.right
            bottom: parent.bottom
            bottomMargin: toolBar.open ? toolBar.height : toolBar.height / 2
        }

        model: _data.pageCount
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        LayoutMirroring.enabled: true
        LayoutMirroring.childrenInherit: false
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 1
        delegate: quranPageDelegate
        currentIndex: settings.pageNumber
        onCurrentIndexChanged: {
            if (currentIndex != -1) {
                settings.pageNumber = currentIndex
            }
        }
    }

    Component {
        id: quranPageDelegate

        QuranView {
        }
    }

    ToolBar {
        id: toolBar
    }
}
