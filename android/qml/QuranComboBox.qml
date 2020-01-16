// -*- qml -*-
/*
 * Copyright (c) 2011-2020 Mohammed Sameer <msameer@foolab.org>.
 *
 * This package is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

import QtQuick 2.2
import QtQuick.Controls 2.2

Column {
    id: item

    property var model
    property alias label: title.text
    property int currentIndex
    property var textRole: function(model) { return model.text }

    width: parent.width

    function __setSubtitle() {
        var o = collection.objectAt(item.currentIndex)
        if (o) {
            subtitle.text = item.textRole(o.data)
        }
    }

    Component.onCompleted: __setSubtitle()
    onCurrentIndexChanged: __setSubtitle()

    Instantiator {
        id: collection
        model: item.model
        delegate: QtObject {
            property var data: model
        }

        onCountChanged: __setSubtitle()
    }

    QuranLabel {
        id: title
        height: quranTheme.sizes.small
        anchors {
            right: parent.right
            left: parent.left
            margins: quranTheme.sizes.marginSmall
        }

        color: quranTheme.colors.primary
    }

    QuranBackgroundItem {
        width: parent.width
        height: subtitle.height + (2 * quranTheme.sizes.marginSmall)
        onClicked: {
            var dlg = dialog.createObject(stack.currentItem)
            dlg.open()
        }

        QuranLabel {
            id: subtitle
            height: quranTheme.sizes.small
            verticalAlignment: Text.AlignVCenter
            anchors {
                top: parent.top
                topMargin: quranTheme.sizes.marginSmall
                right: parent.right
                rightMargin: quranTheme.sizes.marginMedium
                left: parent.left
                leftMargin: quranTheme.sizes.marginMedium
            }

            color: parent.highlighted ? quranTheme.colors.secondaryHighlight : quranTheme.colors.secondary
        }
    }

    Component {
        id: dialog

        QuranPopup {
            id: popup
            parent: stack.currentItem
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            height: Math.min(parent.height - (2 * margins), collection.count * quranTheme.sizes.itemSmall + popupTitle.height + (2 * margins))
            onAboutToShow: view.positionViewAtIndex(item.currentIndex, ListView.Center)

            contentItem: Column {
                width: parent.width
                height: parent.height

                QuranLabel {
                    id: popupTitle
                    font.pixelSize: quranTheme.fonts.tiny
                    color: quranTheme.colors.secondary
                    anchors {
                        right: parent.right
                        left: parent.left
                        margins: quranTheme.sizes.marginSmall
                    }

                    text: title.text
                    verticalAlignment: Text.AlignVCenter
                }

                QuranListView {
                    id: view
                    boundsBehavior: Flickable.StopAtBounds
                    width: parent.width
                    height: parent.height - popupTitle.height
                    clip: true
                    model: item.model

                    delegate: QuranBackgroundItem {
                        function _color(index) {
                            if (item.currentIndex == index) {
                                return highlighted ? quranTheme.colors.secondaryHighlight : quranTheme.colors.secondary
                            } else {
                                return highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
                            }
                        }

                        width: parent.width
                        height: quranTheme.sizes.itemSmall
                        // For an unknown reason; binding anything to currentIndex makes
                        // QML barf with:
                        // qrc:/qml/QuranComboBox.qml:139: ReferenceError: popup is not defined
                        // so we try to work around it.
                        property Popup __popup: null
                        Component.onCompleted: __popup = popup
                        onClicked: {
                            item.currentIndex = index
                            __popup.close()
                            __popup.destroy(200)
                        }

                        QuranLabel {
                            anchors {
                                fill: parent
                                margins: quranTheme.sizes.marginSmall
                            }
                            text: item.textRole(model)
                            verticalAlignment: Text.AlignVCenter
                            color: parent._color(index)
                        }
                    }
                }
            }
        }
    }
}
