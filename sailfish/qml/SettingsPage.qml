// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

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
                                        remorse.execute("Resetting", function() { settings.reset() })
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
                                        font.family: settings.fontFamily
                                        font.pixelSize: settings.fontSize
                                        color: theme.textColor
                                        wrapMode: Text.WordWrap

                                        Component.onCompleted: populate();

                                        NumberFormatter {
                                                id: formatter
                                                format: settings.numberFormat
                                                number: 1
                                        }

                                        function populate() {
                                                text = qsTr("%1 (%2)").arg(_data.basmala).arg(formatter.formattedNumber)
                                        }

                                        Connections {
                                                target: settings
                                                onNumberFormatChanged: previewLabel.populate();
                                                onTextTypeChanged: previewLabel.populate();
                                        }
                                }
                        }

                        Slider {
                                width: parent.width
                                label: qsTr("Text size")
                                minimumValue: settings.minFontSize
                                maximumValue: settings.maxFontSize
                                valueText: value
                                stepSize: 1
                                value: settings.fontSize
                                onValueChanged: settings.fontSize = value
                        }

                        Slider {
                                width: parent.width
                                label: qsTr("Translation text size");
                                minimumValue: settings.minTranslationFontSize
                                maximumValue: settings.maxTranslationFontSize
                                valueText: value
                                stepSize: 1
                                value: settings.translationFontSize
                                onValueChanged: settings.translationFontSize = value
                        }

                        TextSwitch {
                                text: qsTr("Use simplified text")
                                checked: settings.textType == 1
                                onCheckedChanged: settings.textType = checked ? 1 : 0
                        }

                        TextSwitch {
                                text: qsTr("Use Arabic numerals")
                                checked: settings.numberFormat == 1
                                onCheckedChanged: settings.numberFormat = checked ? 1 : 0
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Automatic") }
                                        MenuItem { text: qsTr("Portrait") }
                                        MenuItem { text: qsTr("Landscape") }
                                }

                                label: qsTr("Orientation")
                                currentIndex: settings.orientation
                                onCurrentIndexChanged: settings.orientation = currentIndex
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Disabled") }
                                        MenuItem { text: qsTr("Enabled") }
                                        MenuItem { text: qsTr("Hidden") }
                                }

                                label: qsTr("Translation")
                                currentIndex: settings.translationMode
                                onCurrentIndexChanged: settings.translationMode = currentIndex
                                enabled: _translations.installedCount > 0
                        }

                        TextSwitch {
                                text: qsTr("Night mode")
                                checked: settings.nightMode
                                onCheckedChanged: settings.nightMode = checked
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Automatic") }
                                        MenuItem { text: qsTr("Center") }
                                }

                                label: qsTr("Text alignment");
                                currentIndex: settings.centerText ? 1 : 0;
                                onCurrentIndexChanged: settings.centerText = (currentIndex == 1);
                        }

                        TextSwitch {
                                text: qsTr("Enable recitations")
                                checked: settings.recitationMode == 1
                                onCheckedChanged: settings.recitationMode = checked ? 1 : 0
                                enabled: _recitations.installedCount > 0
                        }

                        TextSwitch {
                                text: qsTr("Flip phone to stop recitation")
                                checked: settings.flipToStopRecitation
                                onCheckedChanged: settings.flipToStopRecitation = checked
                        }
                }
        }
}
