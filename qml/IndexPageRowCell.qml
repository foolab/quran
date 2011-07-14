/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

import QtQuick 1.1
import com.nokia.meego 1.0

Rectangle {
  height: name.height
  property int sIndex: 0

  NumberLabel {
    width: parent.width/4
    id: number
    number: sIndex + 1
    anchors.left: name.right
    horizontalAlignment: Text.AlignRight
    // TODO: Seems not to be aligned properly with the button
    verticalAlignment: Text.AlignVCenter
  }

  QuranPageScrollButton {
    id: name
    _sura: sIndex
    _aya: 0
    width: (parent.width/4) * 3
    text: _data.suraName(sIndex)
  }
}
