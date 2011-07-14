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
  id: settingspage
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
      contentHeight: numberFormat.y + numberFormat.height + numberFormat.anchors.topMargin
      height: parent.height
      width: parent.width

      Label {
        id: settings
        anchors.top: parent.top
        anchors.topMargin: 16
        anchors.left: parent.left
        anchors.leftMargin: 16
        text: qsTr("Settings")
        font.pixelSize: 32
      }

      Label {
        id: previewLabel
        anchors.top: settings.bottom
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 26
        font.pixelSize: 26
        text: qsTr("Preview")
      }

      Label {
        // HACK: Seems the only way to listen to _settings.numberFormat
        // and _settings.textType is to alias them to properties!
        property int format: _settings.numberFormat
        property int textType: _settings.textType

        id: preview
        anchors.top: previewLabel.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 16
        horizontalAlignment: Text.AlignHCenter
        font.family: _settings.fontFamily
        font.pointSize: _settings.fontSize

        onFormatChanged: populate();
        onTextTypeChanged: populate();
        Component.onCompleted: populate();

        function populate() {
          text = _data.text(0, 0) + " (" + _formatter.number(1) + ")"
        }
      }

      Label {
        id: fontSizeLabel
        anchors.top: preview.bottom
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 26
        font.pixelSize: 26
        text: qsTr("Font size")
      }

      Row {
        id: sliderRect
        anchors.top: fontSizeLabel.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter

        Label {
            text: fontSize.value
            height: fontSize.height
            verticalAlignment: Text.AlignVCenter
        }

        Slider {
          id: fontSize
          orientation: Qt.Horizontal
          stepSize: 1
          valueIndicatorVisible: true
          value: _settings.fontSize
          minimumValue: _settings.minFontSize
          maximumValue: _settings.maxFontSize

          // HACK: valueChanged will be emitted upon the component creation with value
          // being the minimum (16) and we end up resetting the settings.
          // We will only set it now if the component has been created.
          // We cannot use Component.progress or Component.status because
          // both are returning "undefined"
          property bool done: false
          onValueChanged: {
            if (done) {
              _settings.fontSize = value
            }
          }

          Component.onCompleted: done = true
        }
      }

      Label {
        id: textTypeLabel
        anchors.topMargin: 26
        anchors.left: parent.left
        anchors.leftMargin: 26
        font.pixelSize: 26
        anchors.top: sliderRect.bottom
        text: qsTr("Text type")
      }

      ButtonRow {
        id: textType
        anchors.top: textTypeLabel.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
          id: uthmani
          text: qsTr("Uthmani");
          onClicked: _settings.textType = 0;
          checked: (_settings.textType == 0);
        }

        Button {
          id: simple
          text: qsTr("Simple");
          onClicked: _settings.textType = 1;
          checked: (_settings.textType == 1);
        }
      }

      Label {
        id: numberFormatLabel
        anchors.top: textType.bottom
        anchors.left: parent.left
        anchors.leftMargin: 26
        anchors.topMargin: 26
        font.pixelSize: 26
        text: qsTr("Number format")
      }

      ButtonRow {
        id: numberFormat
        anchors.top: numberFormatLabel.bottom
        anchors.topMargin: 6
        anchors.horizontalCenter: parent.horizontalCenter
        Button {
          id: hindi
          text: qsTr("Hindi");
          onClicked: _settings.numberFormat = 0;
          checked: (_settings.numberFormat == 0);
        }

        Button {
          id: arabic
          text: qsTr("Arabic");
          onClicked: _settings.numberFormat = 1;
          checked: (_settings.numberFormat == 1);
        }
      }
    }
  }
}
