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

// TODO: port
Page {
  id: aboutpage
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
        iconId: "toolbar-back"
        onClicked: pageStack.pop();
      }
    }

    Flickable {
      id: flick
      anchors.fill: parent
      width: parent.width
      contentHeight: col.height
      anchors.topMargin: 16
      anchors.bottomMargin: 16

      Column {
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.leftMargin: 26
        anchors.rightMargin: 26
        id: col
        spacing: 16
        width: parent.width

        Image {
          id: logo
          anchors.horizontalCenter: parent.horizontalCenter
          source: "image://quran/logo.svg"
          cache: false
          width: 160
          height: 160
        }

        Label {
          id: name
          text: qsTr("Holy Quran Version ") + _settings.version;
          anchors.horizontalCenter: parent.horizontalCenter
          font.pixelSize: 36
          font.bold: true
          width: parent.width
          horizontalAlignment: Text.AlignHCenter
        }

        Label {
          id: copy
          text: _legal.copyright
          anchors.horizontalCenter: parent.horizontalCenter
          horizontalAlignment: Text.AlignHCenter
          width: parent.width
          wrapMode: Text.WordWrap
          textFormat: Text.RichText
        }

        Rectangle {
          height: gpl.height + gplLabel.height
          width: parent.width

          Label {
            id: gplLabel
            font.bold: true
            text: qsTr("Application license:");
          }

          Label {
            id: gpl
            text: _legal.appLicense
            wrapMode: Text.WordWrap
            anchors.top: gplLabel.bottom
            width: parent.width
            textFormat: Text.RichText
          }
        }

        Rectangle {
          width: parent.width
          height: data.height + dataLabel.height

          Label {
            id: dataLabel
            font.bold: true
            text: qsTr("Text license:");
          }

          Label {
            id: data
            text: _legal.textLicense
            wrapMode: Text.WordWrap
            anchors.top: dataLabel.bottom
            width: parent.width
            textFormat: Text.RichText
          }
        }

        Rectangle {
          width: parent.width
          height: fnt.height + fntLabel.height

          Label {
            id: fntLabel
            font.bold: true
            text: qsTr("Font license:");
          }

          Label {
            id: fnt
            text: _legal.fontLicense
            wrapMode: Text.WordWrap
            anchors.top: fntLabel.bottom
            width: parent.width
            textFormat: Text.RichText
          }
        }
      }
    }
  }
}
