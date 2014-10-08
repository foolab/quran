// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

QuranPage {
        id: settingsPage

        RemorsePopup { id: remorse }

        SilicaFlickable {
                id: flick
                anchors.fill: parent
                contentHeight: col.height

                PullDownMenu {
                        MenuItem {
                                text: qsTr("Reset")
                                onClicked: {
                                        remorse.execute("Resetting", function() { _settings.reset() })
                                }
                        }

                        MenuItem {
                                text: qsTr("Manage recitations")
                                onClicked: pageStack.push(Qt.resolvedUrl("RecitationsListPage.qml"))
                        }

                        MenuItem {
                                text: qsTr("Manage translations")
                                onClicked: pageStack.push(Qt.resolvedUrl("TranslationsListPage.qml"))
                        }
                }

                Column {
                        id: col
                        width: parent.width

                        PageHeader {
                                width: parent.width
                                title: qsTr("Settings")
                        }

                        Rectangle {
                                width: parent.width
                                height: previewLabel.height
                                color: theme.backgroundColor

                                Label {
                                        id: previewLabel
                                        width: parent.width
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: _settings.fontFamily
                                        font.pixelSize: _settings.fontSize
                                        color: theme.textColor
                                        wrapMode: Text.WordWrap

                                        Component.onCompleted: populate();

                                        function populate() {
                                                text = qsTr("%1 (%2)").arg(_data.basmala).arg(_formatter.number(1))
                                        }

                                        Connections {
                                                target: _settings
                                                onNumberFormatChanged: previewLabel.populate();
                                                onTextTypeChanged: previewLabel.populate();
                                        }
                                }
                        }

                        Slider {
                                width: parent.width
                                label: qsTr("Text size")
                                minimumValue: _settings.minFontSize
                                maximumValue: _settings.maxFontSize
                                valueText: value
                                stepSize: 1
                                value: _settings.fontSize
                                onValueChanged: _settings.fontSize = value
                        }

                        Slider {
                                width: parent.width
                                label: qsTr("Translation text size");
                                minimumValue: _settings.minTranslationFontSize
                                maximumValue: _settings.maxTranslationFontSize
                                valueText: value
                                stepSize: 1
                                value: _settings.translationFontSize
                                onValueChanged: _settings.translationFontSize = value
                        }

                        TextSwitch {
                                text: qsTr("Use simplified text")
                                checked: _settings.textType == 1
                                onCheckedChanged: _settings.textType = checked ? 1 : 0
                        }

                        TextSwitch {
                                text: qsTr("Use Arabic numerals")
                                checked: _settings.numberFormat == 1
                                onCheckedChanged: _settings.numberFormat = checked ? 1 : 0
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Automatic") }
                                        MenuItem { text: qsTr("Portrait") }
                                        MenuItem { text: qsTr("Landscape") }
                                }

                                label: qsTr("Orientation")
                                currentIndex: _settings.orientation
                                onCurrentIndexChanged: _settings.orientation = currentIndex
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Disabled") }
                                        MenuItem { text: qsTr("Enabled") }
                                        MenuItem { text: qsTr("Hidden") }
                                }

                                label: qsTr("Translation")
                                currentIndex: _settings.translationMode
                                onCurrentIndexChanged: _settings.translationMode = currentIndex
                                enabled: _translations.installedCount > 0
                        }

                        TextSwitch {
                                text: qsTr("Night mode")
                                checked: _settings.nightMode
                                onCheckedChanged: _settings.nightMode = checked
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Automatic") }
                                        MenuItem { text: qsTr("Center") }
                                }

                                label: qsTr("Text alignment");
                                currentIndex: _settings.centerText ? 1 : 0;
                                onCurrentIndexChanged: _settings.centerText = (currentIndex == 1);
                        }

                        TextSwitch {
                                text: qsTr("Enable recitations")
                                checked: _settings.recitationMode == 1
                                onCheckedChanged: _settings.recitationMode = checked ? 1 : 0
                                enabled: _recitations.installedCount > 0
                        }

                        TextSwitch {
                                text: qsTr("Flip phone to stop recitation")
                                checked: _settings.flipToStopRecitation
                                onCheckedChanged: _settings.flipToStopRecitation = checked
                        }
                }
        }
}
