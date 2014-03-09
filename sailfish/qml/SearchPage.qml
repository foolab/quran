// -*- qml -*-
import QtQuick 2.0
import Sailfish.Silica 1.0
import Quran 1.0

QuranPage {
        id: searchPage

        property string searchString

        function doSearch() {
            searchModel.setQuery(searchPage.searchString, _settings.searchMatchWholeWords)
        }

        Component {
                id: headerDelegate

                Column {
                        width: parent.width

                        PageHeader {
                                width: parent.width
                                title: qsTr("Search")
                        }

                        SearchField {
                                width: parent.width
                                horizontalAlignment: TextInput.AlignRight
                                onTextChanged: searchPage.searchString = text
                                EnterKey.onClicked: {
                                    if (searchPage.searchString.length) {
                                        searchPage.doSearch()
                                    }
                                }
                        }

                        TextSwitch {
                                width: parent.width
                                text: qsTr("Match whole words only");
                                checked: _settings.searchMatchWholeWords
                                onCheckedChanged: _settings.searchMatchWholeWords = checked
                        }
                }
        }

        // TODO: merge all those delegates with the favorites page
        Component {
                id: sectionDelegate

                Label {
                        id: title
                        anchors.right: parent.right
                        anchors.rightMargin: 20
                        font.family: Theme.fontFamily
                        horizontalAlignment: Text.AlignRight
                        verticalAlignment: Text.AlignVCenter
                        text: _data.fullSuraName(section)
                        color: Theme.highlightColor
                }
        }

        Component {
                id: searchPageDelegate

                BackgroundItem {
                        id: item
                        width: parent.width
                        height: label.height

                        onClicked: {
                                pagePosition.setPosition(chapter, verse)
                                pageStack.pop()
                        }

                        Label {
                                id: label
                                anchors {
                                        top: parent.top
                                        right: parent.right
                                        left: parent.left
                                        rightMargin: 16
                                        leftMargin: 16
                                }

                                font.family: _settings.fontFamily
                                text: qsTr("(%1) %2").arg(_formatter.number(verse + 1)).arg(_data.text(chapter, verse))
                                wrapMode: Text.WordWrap
                                horizontalAlignment: Text.AlignRight
                                color: Theme.primaryColor
                        }
                }
        }

        SilicaListView {
                id: view
                anchors.fill: parent
                header: headerDelegate

                ViewPlaceholder {
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
}
