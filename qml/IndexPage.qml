// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

QuranPage {
    property list<QtObject> tabs: [
        QtObject {
            property string name: qsTr("Chapter")
            property string longName: qsTr("Choose chapter")
            property Component component: chapterComponent
        },

        QtObject {
            property string name: qsTr("Part")
            property string longName: qsTr("Choose part")
            property Component component: partComponent
        },

        QtObject {
            property string name: qsTr("Page")
            property string longName: qsTr("Choose page")
            property Component component: pageComponent
        }
    ]

    QuranPageHeader {
        id: pageHeader

        width: parent.width
        title: tabs[mainView.currentIndex].longName
    }

    Row {
        id: tabSelector
        anchors.top: pageHeader.bottom
        width: parent.width
        Repeater {
            model: tabs
            delegate: QuranBackgroundItem {
                width: parent.width / 3
                height: quranTheme.itemSizeSmall
                onClicked: mainView.currentIndex = index
                    QuranLabel {
                        anchors.fill: parent
                        color: index == mainView.currentIndex ? quranTheme.highlightColor : quranTheme.primaryColor
                        text: name
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                }
            }
        }
    }

    function repositionView(view) {
        view.positionViewAtIndex(view.currentIndex, ListView.Center)
    }

    QuranListView {
        id: mainView
        anchors {
            top: tabSelector.bottom
            bottom: parent.bottom
            left: parent.left
            right: parent.right
        }

        model: tabs
        orientation: ListView.Horizontal
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        highlightFollowsCurrentItem: true
        highlightMoveDuration: 1

        delegate: Loader {
            sourceComponent: component
            width: ListView.view.width
            height: ListView.view.height
        }
    }

    Component {
        id: chapterComponent

        QuranListView {
            id: chapterView
            clip: true
            model: _data.verseCount
            highlightFollowsCurrentItem: false
            currentIndex: _data.firstSuraForPage(settings.pageNumber)

            onWidthChanged: repositionView(chapterView)
            onHeightChanged: repositionView(chapterView)
            Component.onCompleted: repositionView(chapterView)

            delegate: IndexDelegate {
                number: index
                text: _data.fullSuraName(index)
                highlight: ListView.isCurrentItem
                showPlay: settings.recitationMode != 0 && _recitations.installedCount > 0
                onClicked: {
                    pagePosition.setPosition(index, 0)
                    popPage()
                }

                onPlayClicked: {
                    pagePosition.setPosition(index, 0)
                    playAudio(MediaPlayer.PlayChapter, index)
                    popPage()
                }

                onMoreClicked: {
                    pushAnimated(Qt.resolvedUrl("VerseSelectionPage.qml"), {chapter: number})
                }
            }
        }
    }

    Component {
        id: partComponent

        QuranListView {
            id: partView
            clip: true
            model: _data.partCount
            anchors.fill: parent
            highlightFollowsCurrentItem: false
            currentIndex: _data.partNumberForPage(settings.pageNumber)

            onWidthChanged: repositionView(partView)
            onHeightChanged: repositionView(partView)
            Component.onCompleted: repositionView(partView)

            delegate: IndexDelegate {
                number: index
                text: _data.partName(index)
                showPlay: settings.recitationMode != 0 && _recitations.installedCount > 0
                showMore: false
                highlight: ListView.isCurrentItem

                onClicked: {
                    settings.pageNumber = _data.pageNumberForPart(index)
                    popPage()
                }

                onPlayClicked: {
                    settings.pageNumber = _data.pageNumberForPart(index)
                    playAudio(MediaPlayer.PlayPart, index)
                    popPage()
                }
            }
        }
    }

    Component {
        id: pageComponent
        Item {
            anchors.fill: parent

            Column {
                width: parent.width
                anchors.centerIn: parent
                QuranLabel {
                    anchors {
                        left: parent.left
                        right: parent.right
                        leftMargin: quranTheme.marginMedium
                        rightMargin: quranTheme.marginMedium
                    }

                    text: qsTr("Enter a page number between 1 and %1").arg(_data.pageCount)
                    font.pixelSize: quranTheme.fontSizeLarge
                    color: quranTheme.primaryColor
                }

                QuranTextField {
                    anchors.horizontalCenter: parent.horizontalCenter
                    text: settings.pageNumber + 1
                    width: parent.width / 2
                    focus: true
                    inputMethodHints: Qt.ImhDigitsOnly
                    validator: IntValidator { bottom: 1; top: _data.pageCount }

                    font {
                        pixelSize: quranTheme.fontSizeLarge
                        family: quranTheme.fontFamilyHeading
                    }

                    enterKeyEnabled: acceptableInput
                    enterKeyText: qsTr("Go!")
                    onEnterKeyClicked: {
                        if (acceptableInput) {
                            settings.pageNumber = text - 1
                            popPage()
                        }
                    }
                }
            }
        }
    }
}
