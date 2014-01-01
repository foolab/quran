// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

Page {
        id: aboutPage

        tools: toolBar

        AboutData {
                id: _about
        }

        Flickable {
                id: flick
                anchors.top: parent.top
                anchors.bottom: toolBar.top
                anchors.left: parent.left
                anchors.right: parent.right
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
                                source: "qrc:/logo.png"
                                //          cache: false
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
                                color: _colors.textColor
                        }

                        Label {
                                id: copy
                                text: _about.copyright
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter
                                width: parent.width
                                wrapMode: Text.WordWrap
                                textFormat: Text.RichText
                                color: _colors.textColor
                        }


                        AboutPageBlock {
                                id: contributors
                                title: qsTr("Contributors:");
                                text: _about.contributors
                        }

                        AboutPageBlock {
                                id: appLicense
                                title: qsTr("Application license:");
                                text: _about.appLicense
                        }

                        AboutPageBlock {
                                id: textLicense
                                title: qsTr("License for simplified text:");
                                text: _about.textLicense
                        }

                        AboutPageBlock {
                                id: translationsLicense
                                title: qsTr("Translations license:");
                                text: _about.transLicense
                        }
                }
        }

        ToolBar {
                id: toolBar
                ToolBarLayout {
                        ToolButton { icon: theme.pageBack; onClicked: pageStack.pop(); }
                }
        }
}
