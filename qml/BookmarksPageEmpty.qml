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
  id: bookmarksemptypage
  orientationLock: PageOrientation.LockPortrait
  anchors.fill: parent
  tools: toolBar

  Rectangle {
    color: "white"
    width: parent.width
    height: parent.height

    ToolBarLayout {
      id: toolBar
      ToolIcon {
        id: back
        iconId: "toolbar-back"
        onClicked: pageStack.pop();
      }
    }

    Label {
      id: title
      text: qsTr("Favorites")
      font.pixelSize: 32
      anchors.top: parent.top
      anchors.topMargin: 16
      anchors.left: parent.left
      anchors.leftMargin: 16
    }

    Label {
      id: error
      text: qsTr("No favorites. Long tap on a verse to add or remove favorites")
      anchors.left: parent.left
      anchors.leftMargin: 16
      anchors.right: parent.right
      anchors.rightMargin: 16
      anchors.top: title.bottom
      anchors.topMargin: 26
      width: parent.width
      font.pixelSize: 26
    }
  }
}
