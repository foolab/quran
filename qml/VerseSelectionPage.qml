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
    id: page
    property int chapter
    ChapterInfo {
        id: info
        chapter: page.chapter
    }

    QuranGridView {
        id: grid
        model: info.length
        cellWidth: grid.width / 7
        cellHeight: cellWidth

        anchors {
            fill: parent
            leftMargin: quranTheme.marginMedium
            rightMargin: quranTheme.marginMedium
        }

        header: QuranPageHeader {
            width: grid.width
            title: qsTr("Choose a verse")
        }

        delegate: QuranBackgroundItem {
            width: grid.cellWidth
            height: grid.cellHeight

            NumberLabel {
                anchors.fill: parent
                color: quranTheme.primaryColor
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                number: index + 1
                enableSignals: false
            }

            onClicked: {
                pagePosition.setPosition(page.chapter, index)
                popToFirst()
            }
        }
    }
}
