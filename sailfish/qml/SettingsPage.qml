// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0

Page {
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
                                color: _colors.backgroundColor

                                Label {
                                        id: previewLabel
                                        width: parent.width
                                        anchors.horizontalCenter: parent.horizontalCenter
                                        horizontalAlignment: Text.AlignHCenter
                                        font.family: _settings.fontFamily
                                        font.pixelSize: _settings.fontSize
                                        color: _colors.textColor
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
                                label: qsTr("Translation font size");
                                minimumValue: _settings.minTranslationFontSize
                                maximumValue: _settings.maxTranslationFontSize
                                valueText: value
                                stepSize: 1
                                value: _settings.translationFontSize
                                onValueChanged: _settings.translationFontSize = value
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Uthmani") }
                                        MenuItem { text: qsTr("Simple") }
                                }

                                label: qsTr("Text type")
                                currentIndex: _settings.textType
                                onCurrentIndexChanged: _settings.textType = currentIndex
                        }

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Hindi") }
                                        MenuItem { text: qsTr("Arabic") }
                                }

                                label: qsTr("Number format")
                                currentIndex: _settings.numberFormat
                                onCurrentIndexChanged: _settings.numberFormat = currentIndex
                        }
/*
                        SettingsPageEntry {
                                id: orientationEntry
                                entries: [
                                QtObject {property string name: qsTr("Automatic") },
                                QtObject {property string name: qsTr("Portrait") },
                                QtObject {property string name: qsTr("Landscape") }
                                ]

                                title: qsTr("Orientation")
                                subtitle: entries[_settings.orientation].name;
                                selectedIndex: _settings.orientation
                                onAccepted: _settings.orientation = selectedIndex;
                        }
*/

                        ComboBox {
                                menu: ContextMenu {
                                        MenuItem { text: qsTr("Disabled") }
                                        MenuItem { text: qsTr("Enabled") }
                                        MenuItem { text: qsTr("Hidden") }
                                }

                                label: qsTr("Translation")
                                currentIndex: _settings.translationMode
                                onCurrentIndexChanged: _settings.translationMode = currentIndex
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
/*
                        SettingsPageEntry {
                                id: recitationModeEntry
                                entries: [
                                QtObject {property string name: qsTr("Disabled") },
                                QtObject {property string name: qsTr("Enabled") }
                                ]

                                title: qsTr("Recitation")
                                subtitle: entries[_settings.recitationMode].name;
                                selectedIndex: _settings.recitationMode
                                onAccepted: _settings.recitationMode = selectedIndex;
                       }

                        SettingsPageEntry {
                                id: recitationsEntry
                                title: qsTr("Show recitations")
                                onClicked: pageStack.push("RecitationsListPage");
                                subtitle: _recitations.installed.length + qsTr(" installed.");
                        }

                        SettingsPageEntry {
                                id: flipToStopRecitation
                                entries: [
                                QtObject {property string name: qsTr("Yes") },
                                QtObject {property string name: qsTr("No") }
                                ]

                                title: qsTr("Flip phone to stop recitation")
                                subtitle: entries[selectedIndex].name;
                                selectedIndex: _settings.flipToStopRecitation ? 0 : 1
                                onAccepted: _settings.flipToStopRecitation = (selectedIndex == 0)
                        }

                        SettingsPageEntry {
                                id: applicationTheme
                                title: qsTr("Theme")
                                subtitle: _settings.theme
                                onClicked: pageStack.push("ThemeListPage");
                                visible: _theme.themes().length > 1
                        }
*/
                }
        }
}
