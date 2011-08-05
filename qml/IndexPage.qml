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

Page {
  id: indexpage
  orientationLock: PageOrientation.LockPortrait
  anchors.fill: parent
  tools: toolBar

  signal scrollTo(int sura, int aya);

  Rectangle {
    color: "white"
    width: parent.width
    height: parent.height

    ToolBarLayout {
      id: toolBar
      ToolIcon {
        iconId: "toolbar-back"
        onClicked: pageStack.pop();
      }
    }

    ListView {
      id: view
      anchors.fill: parent
      model: _data.suraCount()/2
      delegate: row

      Component {
        id: row
        IndexPageRow {
          width: view.width;
          rowIndex: index
        }
      }
    }
  }
}
