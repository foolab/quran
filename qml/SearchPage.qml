// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
        id: searchPage

        property string searchString
        property Item searchField

        function doSearch() {
            searchModel.setQuery(searchPage.searchString, settings.searchMatchWholeWords)
        }

        Component {
                id: headerDelegate

                Column {
                        width: parent.width

                        QuranPageHeader {
                                width: parent.width
                                title: qsTr("Search")
                        }

                        SearchField {
                                id: field
                                Component.onCompleted: searchPage.searchField = field
                                width: parent.width
                                horizontalAlignment: TextInput.AlignRight
                                onTextChanged: searchPage.searchString = text
                                enableSoftwareInputPanel: false
                                EnterKey.onClicked: {
                                    if (searchPage.searchString.length) {
                                        searchPage.doSearch()
                                    }
                                }
                        }

                        QuranTextSwitch {
                                width: parent.width
                                text: qsTr("Match whole words only");
                                checked: settings.searchMatchWholeWords
                                onCheckedChanged: settings.searchMatchWholeWords = checked
                        }
                }
        }

        // TODO: merge all those delegates with the favorites page
        Component {
                id: sectionDelegate

                QuranLabel {
                        id: title
                        anchors.right: parent.right
                        anchors.rightMargin: theme.largeMedium
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: _data.fullSuraName(section)
                        color: Theme.highlightColor
                }
        }

        Component {
                id: searchPageDelegate

                QuranBackgroundItem {
                        id: item
                        width: parent.width
                        height: label.height

                        onClicked: {
                                pagePosition.setPosition(chapter, verse)
                                pageStack.pop()
                        }

                        QuranLabel {
                                id: label
                                anchors {
                                        top: parent.top
                                        right: parent.right
                                        left: parent.left
                                        rightMargin: theme.marginMedium
                                        leftMargin: theme.marginMedium
                                }

                                NumberFormatter {
                                        id: formatter
                                        format: settings.numberFormat
                                        number: verse + 1
                                }

                                font.family: settings.fontFamily
                                text: qsTr("(%1) %2").arg(formatter.formattedNumber).arg(_data.text(chapter, verse))
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignRight
                                color: Theme.primaryColor
                        }
                }
        }

        QuranListView {
                id: view
                anchors.fill: parent
                anchors.bottomMargin: keyboard.visible ? keyboard.height : 0
                clip: keyboard.visible
                header: headerDelegate

                QuranViewPlaceholder {
                        text: searchModel.errorString
                        enabled: searchModel.error
                }

                PullDownMenu {
                        visible: searchPage.searchString.length > 0

                        MenuItem {
                                text: qsTr("Search")
                                onClicked: searchPage.doSearch()
                        }
                }

                section.property: "chapter"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate

                delegate: searchPageDelegate

                model: SearchModel {
                    id: searchModel
                }
        }

        Rectangle {
            id: preview
            property alias text: label.text
            width: Theme.itemSizeLarge
            height: width
            color: Theme.highlightColor
            visible: text != ""

            anchors {
                bottom: keyboard.top
                horizontalCenter: parent.horizontalCenter
            }

            QuranLabel {
                color: Theme.primaryColor
                id: label
                anchors.fill: parent
                font.pixelSize: Theme.fontSizeLarge
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
            }
        }

        Keyboard {
            id: keyboard
            anchors {
                bottom: parent.bottom
                right: parent.right
                left: parent.left
            }

            opacity: searchPage.searchField.focus ? 1.0 : 0
            visible: opacity > 0

            Behavior on opacity {
                NumberAnimation { duration: 200 }
            }

            onKeyPressed: {
                if (code != " ") {
                    preview.text = code
                }
            }

            onKeyReleased: preview.text = ""
            onKeyClicked: searchField.text = searchField.text + code
            onBackspaceClicked: searchField.text = searchField.text.slice(0, -1)
            onEnterClicked: {
                if (searchPage.searchString.length) {
                    searchPage.doSearch()
                    searchPage.searchField.focus = false
                }
            }
        }

}
