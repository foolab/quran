// -*- qml-mode -*-
import QtQuick 1.0

Page {
        id: searchPage

        tools: toolBar

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Search")
        }

        Component {
                id: headerDelegate

                Column {
                        spacing: 16
                        TextField {
                                id: query
                                width: view.width
                                placeholderText: qsTr("Search");
                                anchors.horizontalCenter: parent.horizontalCenter
                                horizontalAlignment: TextInput.AlignRight
                        }

                        DialogButton {
                                id: search
                                text: qsTr("Search");
                                anchors.horizontalCenter: parent.horizontalCenter
                                onClicked: {
                                        var text = query.text;
                                        var result = _search.search(text);
                                        view.populate(result);
                                }
                        }
                }
        }

        Component {
                id: sectionDelegate

                Rectangle {
                        width: view.width
                        color: _settings.highlightColor
                        height: title.height

                        Label {
                                anchors.right: parent.right
                                anchors.rightMargin: 20
                                id: title
                                font.pointSize: 26
                                font.family: _settings.fontFamily
                                horizontalAlignment: Text.AlignRight
                                verticalAlignment: Text.AlignVCenter
                                text: _data.fullSuraName(section)
                        }
                }
        }

        Component {
                id: delegate

                Rectangle {
                        id: rect
                        width: parent.width
                        anchors.left: parent.left
                        anchors.right: parent.right
                        height: Math.max(n.height, ayaText.height);
                        color: index % 2 ? Qt.lighter(_settings.highlightColor, 1.2) : Qt.lighter(_settings.highlightColor, 1.3)

                        NumberLabel {
                                id: n
                                number: aya
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                color: "black"
                                width: 70
                                horizontalAlignment: Text.AlignRight
                        }

                        Button {
                                id: ayaText
                                anchors.verticalCenter: parent.verticalCenter
                                border.width: 0
                                color: parent.color
                                clip: true
                                anchors.left: parent.left
                                anchors.right: n.left
                                font.pointSize: 18
                                font.family: _settings.fontFamily
                                text: _data.text(sura, aya);
                                elide: Text.ElideRight
                                textAlignment: Text.AlignRight
                                onClicked: {
                                        pagePosition.setPosition(sura, aya);
                                        pageStack.pop();
                                }
                        }
                }
        }

        ListView {
                id: view

                clip: true
                anchors.top: title.bottom
                anchors.topMargin: 16
                anchors.left: parent.left
                anchors.leftMargin: 16
                anchors.right: parent.right
                anchors.rightMargin: 16
                anchors.bottom: toolBar.top

                header: headerDelegate

                section.property: "sura"
                section.criteria: ViewSection.FullString
                section.delegate: sectionDelegate
                delegate: delegate

                function populate(result) {
                        model.populate(result);
                }

                model: ListModel {
                        id: model

                        function populate(result) {
                                for (var x = 0; x < result.length; x++) {
                                        var sura = _bookmarks.sura(result[x]);
                                        var aya = _bookmarks.aya(result[x]);
                                        append({"sura": sura, "aya": aya});
                                }
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