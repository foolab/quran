// -*- qml -*-
import QtQuick 1.0

// We can't use simple list models because of https://bugreports.qt.nokia.com//browse/QTBUG-16289

        // TODO: Group similar items together ?
Page {
        id: settingsPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Settings")
        }

        function range(start, end, parentObject) {
                var arr = new Array();

                var str = "import QtQuick 1.0; QtObject { property string name }";
                for (var x = start; x <= end; x++) {
                        var o = Qt.createQmlObject(str, parentObject);
                        o.name = x;
                        arr.push(o);
                }

                return arr;
        }

        Menu {
                id: menu

                MenuLayout {
                        MenuItem { text: qsTr("Reset settings"); onClicked: { menu.close(); resetDialog.open(); } }
                }
        }

        Flickable {
                id: flick
                clip: true
                anchors.top: title.bottom
                anchors.topMargin: 16
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
                contentHeight: col.height

                height: parent.height
                width: parent.width

                Column {
                        id: col
                        width: parent.width

                        Label {
                                id: previewLabel
                                width: parent.width
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter
                                font.family: _settings.fontFamily
                                font.pointSize: _settings.fontSize
                                color: _colors.textColor
                                Component.onCompleted: populate();

                                function populate() {
                                        text = _data.text(0, 0) + " (" + _formatter.number(1) + ")"
                                }

                                Connections {
                                        target: _settings
                                        onNumberFormatChanged: previewLabel.populate();
                                        onTextTypeChanged: previewLabel.populate();
                                }

                        }

                        SettingsPageEntry {
                                id: fontSizeEntry

                                entries: range(_settings.minFontSize, _settings.maxFontSize, fontSizeEntry);
                                title: qsTr("Font size");
                                subtitle: _settings.fontSize
                                selectedIndex: _settings.fontSize - _settings.minFontSize
                                onAccepted: _settings.fontSize = selectedIndex + _settings.minFontSize;
                        }

                        SettingsPageEntry {
                                id: translationFontSizeEntry

                                entries: range(_settings.minTranslationFontSize, _settings.maxTranslationFontSize, translationFontSizeEntry);

                                title: qsTr("Translation font size");
                                subtitle: _settings.translationFontSize
                                selectedIndex: _settings.translationFontSize - _settings.minTranslationFontSize;
                                onAccepted: _settings.translationFontSize = selectedIndex + _settings.minTranslationFontSize;
                        }

                        SettingsPageEntry {
                                id: textTypeEntry
                                entries: [
                                QtObject {property string name: qsTr("Uthmani") },
                                QtObject {property string name: qsTr("Simple") }
                                ]

                                title: qsTr("Text type")
                                subtitle: entries[_settings.textType].name;
                                selectedIndex: _settings.textType;
                                onAccepted: _settings.textType = selectedIndex;
                        }

                        SettingsPageEntry {
                                id: numberFormatEntry
                                property int __number: 123

                                entries: [
                                QtObject {property string name: qsTr("Hindi") },
                                QtObject {property string name: qsTr("Arabic") }
                                ]

                                title: qsTr("Number format")
                                subtitle: entries[_settings.numberFormat].name;
                                selectedIndex: _settings.numberFormat;
                                onAccepted: _settings.numberFormat = selectedIndex;
                        }

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

                        SettingsPageEntry {
                                id: translationModeEntry
                                entries: [
                                QtObject {property string name: qsTr("Disabled") },
                                QtObject {property string name: qsTr("Enabled") },
                                QtObject {property string name: qsTr("Hidden") }
                                ]

                                title: qsTr("Translation")
                                subtitle: entries[_settings.translationMode].name;
                                selectedIndex: _settings.translationMode
                                onAccepted: _settings.translationMode = selectedIndex;
                       }

                        SettingsPageEntry {
                                id: translations
                                title: qsTr("Manage translations")
                                onClicked: pageStack.push("TranslationsListPage");
                                subtitle: _translations.installed.length + qsTr(" installed.");
                       }

/*
                        SettingsPageEntry {
                                // TODO:
                                title: qsTr("Application language");
                                subtitle: "TODO";
                        }
*/

                        SettingsPageEntry {
                                id: nightMode
                                entries: [
                                QtObject {property string name: qsTr("On (Experimental)") },
                                QtObject {property string name: qsTr("Off") }
                                ]
                                title: qsTr("Night mode");
                                subtitle: entries[selectedIndex].name;
                                selectedIndex: _settings.nightMode ? 0 : 1;
                                onAccepted: _settings.nightMode = (selectedIndex == 0);
                        }

                        SettingsPageEntry {
                                id: textAlignmentEntry
                                entries: [
                                QtObject {property string name: qsTr("Automatic") },
                                QtObject {property string name: qsTr("Center") }
                                ]
                                title: qsTr("Text alignment");
                                subtitle: entries[selectedIndex].name;
                                selectedIndex: _settings.centerText ? 1 : 0;
                                onAccepted: _settings.centerText = (selectedIndex == 1);
                        }

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
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                        ToolButton { icon: theme.menuIcon; onClicked: menu.open(); }
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
