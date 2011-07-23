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
  id: bookmarkspage
  orientationLock: PageOrientation.LockPortrait
  anchors.fill: parent
  tools: toolBar

  Connections {
    target: _bookmarks
    onCleared: {
      view.model.clear();
      pageStack.pop();
    }
  }

  Connections {
    target: _bookmarks
    onBookmarkRemoved: view.model.bookmarkRemoved();
  }

  QueryDialog {
    id: clearDialog
    titleText: qsTr("Clear all favorites?")
    acceptButtonText: qsTr("Yes")
    rejectButtonText: qsTr("No")
    onAccepted: _bookmarks.clear();
  }

  QueryDialog {
    property variant bookmark: 0
    id: rmDialog
    titleText: qsTr("Remove favorite?")
    acceptButtonText: qsTr("Yes")
    rejectButtonText: qsTr("No")
    onAccepted: _bookmarks.remove(bookmark)
    function run(b) {
      bookmark = b
      var sura = _bookmarks.sura(bookmark);
      var aya = _bookmarks.aya(bookmark);
      message = _data.text(sura, aya);
      open();
    }
  }

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

      ToolButtonRow {
        ToolButton {
          text: qsTr("Remove all");
          onClicked: clearDialog.open();
        }
      }
    }

    ListView {
      anchors.fill: parent
      anchors.left: parent.left
      anchors.right: parent.right
      anchors.topMargin: 20
      id: view
      model: BookmarksModel {}
      section.property: "sura"
      section.criteria: ViewSection.FullString
      section.delegate: section
      delegate: cell
    }

    Component {
      id: section
      Rectangle {
        width: parent.width
        color: _settings.highlightColor
        height: title.height
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        Label {
          anchors.right: parent.right
          anchors.rightMargin: 20
          id: title
          font.family: _settings.fontFamily
          font.pixelSize: 32
          horizontalAlignment: Text.AlignRight
          text: _data.fullSuraName(section)
        }
      }
    }

    Component {
      id: cell
      Rectangle {
        id: rect
        width: parent.width
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        anchors.top: cell.top
        height: n.height + rm.height
        color: index % 2 ? Qt.lighter(_settings.highlightColor, 1.2) : Qt.lighter(_settings.highlightColor, 1.3)

        // TODO: something with the vertical alignment.
        NumberLabel {
          id: n
          number: aya + 1
          anchors.left: txt.right
          anchors.top: rect.top
          anchors.right: parent.right
          horizontalAlignment: Text.AlignHCenter
//          verticalAlignment: Text.AlignVCenter
        }

        ToolIcon {
          id: rm
          iconId: "toolbar-delete"
          anchors.top: n.bottom
          anchors.right: parent.right
          onClicked: rmDialog.run(bookmark);
        }

        QuranPageScrollButton {
          id: txt
          _sura: sura
          _aya: aya
          anchors.left: parent.left
          anchors.right: rm.left
          font.pixelSize: 26
          font.family: _settings.fontFamily
          text: _data.text(sura, aya);
          height: parent.height
          width: parent.width
        }
      }
    }
  }
}
