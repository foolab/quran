// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
        id: aboutPage

        AboutData {
                id: _about
        }

        QuranFlickable {
                id: flick
                anchors.fill: parent
                contentHeight: col.height
                anchors.topMargin: theme.marginMedium
                anchors.bottomMargin: theme.marginMedium

                Column {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.leftMargin: theme.largeLarge
                        anchors.rightMargin: theme.largeLarge
                        id: col
                        spacing: theme.spacing
                        width: parent.width

                        Image {
                                anchors.horizontalCenter: parent.horizontalCenter
                                source: "qrc:/logo.png"
                                width: 160
                                height: 160
                        }

                        QuranLabel {
                                text: qsTr("Holy Quran Version %1").arg(settings.version)
                                anchors.horizontalCenter: parent.horizontalCenter
                                font.family: theme.fontFamilyHeading
                                font.pixelSize: theme.fontSizeLarge
                                width: parent.width
                                horizontalAlignment: Text.AlignHCenter
                                color: theme.primaryColor
                        }

                        QuranLabel {
                                text: _about.copyright
                                font.pixelSize: theme.fontSizeSmall
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: Text.AlignHCenter
                                width: parent.width
                                wrapMode: Text.WordWrap
                                textFormat: Text.RichText
                                color: theme.primaryColor
                        }

                        AboutPageBlock {
                                title: qsTr("Contributors:")
                                text: _about.contributors
                        }

                        AboutPageBlock {
                                title: qsTr("Application license:")
                                text: _about.appLicense
                        }

                        AboutPageBlock {
                                title: qsTr("License for simplified text:")
                                text: _about.textLicense
                        }

                        AboutPageBlock {
                                title: qsTr("Translations license:")
                                text: _about.transLicense
                        }
                }
        }
}
