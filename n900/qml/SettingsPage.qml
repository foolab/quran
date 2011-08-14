// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: settingsPage

        tools: toolBar

        Flickable {
                id: flick
                anchors.top: parent.top
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
                contentHeight: orientation.y + orientation.height + orientation.anchors.topMargin + orientation.anchors.bottomMargin
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
                        id: preview

                        Connections {
                                target: _settings
                                onNumberFormatChanged: preview.populate();
                                onTextTypeChanged: preview.populate();
                        }

                        // TODO: the rightmost part of the text gets clipped
                        height: isPortrait() ? 200 : 100
                        anchors.top: previewLabel.bottom
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.topMargin: 16
                        anchors.leftMargin: 16
                        anchors.rightMargin: 16
                        horizontalAlignment: Text.AlignHCenter
                        font.family: _settings.fontFamily
                        font.pointSize: _settings.fontSize

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
                      spacing: 5
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

                              onPressedChanged: {
                                      if (!pressed) {
                                      _settings.fontSize = value;
                                      }
                              }

                              // HACK: valueChanged will be emitted upon the
                              // component creation with value
                              // being the minimum (16) and we end up resetting the settings.
                              // We will only set it now if the component has been created.
                              // We cannot use Component.progress or Component.status because
                              // both are returning "undefined"
                              property bool done: false
                              onValueChanged: {
                                      // We also don't update _settings.fontSize while we are
                                      // pressed because we don't want all the components to react
                                      // as changing font size is very slow and affects
                                      // the performance of sliding
                                      if (done && !pressed) {
                                      _settings.fontSize = value
                                      }
                                      else if (pressed) {
                                              preview.font.pointSize = value
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

                Label {
                        id: orientationLabel
                        anchors.top: numberFormat.bottom
                        anchors.left: parent.left
                        anchors.leftMargin: 26
                        anchors.topMargin: 26
                        font.pixelSize: 26
                        text: qsTr("Orientation")
                }

                ButtonRow {
                        id: orientation
                        anchors.top: orientationLabel.bottom
                        anchors.topMargin: 6

                        Button {
                                id: auto
                                text: qsTr("Auto");
                                onClicked: _settings.orientation = 0;
                                checked: (_settings.orientation == 0);
                        }

                        Button {
                                id: portrait
                                text: qsTr("Portrait");
                                onClicked: _settings.orientation= 1;
                                checked: (_settings.orientation == 1);
                        }

                        Button {
                                id: landscape
                                text: qsTr("Landscape");
                                onClicked: _settings.orientation= 2;
                                checked: (_settings.orientation == 2);
                        }
                }
        }

        InfoBanner {
                id: settingsReset
                text: qsTr("Settings reset");
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: "general_backspace"; onClicked: pageStack.pop(); }

                        DialogButton {
                                id: reset;
                                text: qsTr("Reset");
                                onClicked: resetDialog.open();
                                anchors.verticalCenter: parent.verticalCenter
                                // TODO: border or padding
                        }
                }
        }

        QueryDialog {
                id: resetDialog
                titleText: qsTr("Reset settings?")
                acceptButtonText: qsTr("Yes")
                rejectButtonText: qsTr("No")
                onAccepted: { _settings.reset(); settingsReset.show(); }
        }
}
