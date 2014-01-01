// -*- qml -*-
import QtQuick 2.0

Page {
        id: searchPage

        tools: toolBar

        property Component partDialogComponent: null
        property Item partDialog: null

        property Component pageDialogComponent: null
        property Item pageDialog: null

        property Component verseDialogComponent: null
        property Item verseDialog: null

        function showPartDialog() {
                if (!partDialogComponent) {
                        partDialogComponent = Qt.createComponent("PartSelectionDialog.qml");
                        partDialog = partDialogComponent.createObject(searchPage);
                }

                partDialog.open();
        }

        function showPageDialog() {
                if (!pageDialogComponent) {
                        pageDialogComponent = Qt.createComponent("PageSelectionDialog.qml");
                        pageDialog = pageDialogComponent.createObject(searchPage);
                }

                pageDialog.open();
        }

        InfoBanner {
                id: error
                text: qsTr("Error while searching.");
        }

        InfoBanner {
                id: errorOpen
                text: qsTr("Failed to open search database.");
        }

        InfoBanner {
                id: emptyText
                text: qsTr("Search text needed.");
        }


        InfoBanner {
                id: noResults
                text: qsTr("No results found.");
        }

        TitleLabel {
                id: title
                width: parent.width
                anchors.top: parent.top
                text: qsTr("Search")
        }

        Component.onCompleted: _search.enable();
        Component.onDestruction: _search.disable();

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

                        CheckBox {
                                id: matchWhole
                                text: qsTr("Match whole words only");
                                anchors.horizontalCenter: parent.horizontalCenter
                                checked: _settings.searchMatchWholeWords
                        }

                        DialogButton {
                                id: search
                                text: qsTr("Search");
                                anchors.horizontalCenter: parent.horizontalCenter
                                onClicked: {
                                        var text = query.text;
                                        if (text.length == 0) {
                                                emptyText.show();
                                                return;
                                        }

                                        _settings.searchMatchWholeWords = matchWhole.checked;

                                        var result = _search.search(text, matchWhole.checked);
                                        if (result.length == 0) {
                                                view.clearModel();
                                                noResults.show();
                                        }
                                        else if (result[0] == -1) {
                                                view.clearModel();
                                                errorOpen.show();
                                        }
                                        else if (result[0] == -2) {
                                                view.clearModel();
                                                error.show();
                                        }
                                        else {
                                                view.populate(result);
                                        }
                                }
                        }
                }
        }

        // TODO: merge all those delegates with the favorites page and port them to the
        // C++ Label instead of Button.
        Component {
                id: sectionDelegate

                Rectangle {
                        width: view.width
                        color: _colors.sectionColor
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
                        color: _colors.backgroundColor
//                        color: index % 2 ? Qt.lighter(_settings.highlightColor, 1.2) : Qt.lighter(_settings.highlightColor, 1.3)

                        NumberLabel {
                                id: n
                                number: aya
                                anchors.top: parent.top
                                anchors.bottom: parent.bottom
                                anchors.right: parent.right
                                color: _colors.textColor
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
                function clearModel() {
                        model.clear();
                }

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
                        ToolButton { icon: theme.page; onClicked: showPageDialog(); }
                        ToolButton { icon: theme.part; onClicked: showPartDialog(); }
                        ToolButton { icon: theme.index; onClicked: pageStack.replace("IndexPage"); }
                }
        }
}
