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

Item {
  height: cell1.height
  width: parent.width
  property int rowIndex: 0

  IndexPageRowCell {
    id: cell1
    sIndex: rowIndex
    anchors.right: parent.right
    anchors.rightMargin: 20
    width: (parent.width / 2) - 20
  }

  IndexPageRowCell {
    id: cell2
    width: (parent.width / 2) - 20
    sIndex: rowIndex + _data.suraCount()/2
    anchors.right: cell1.left
    anchors.rightMargin: 20
  }
}