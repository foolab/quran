// -*- qml -*-
/*
 * Copyright (c) 2011-2017 Mohammed Sameer <msameer@foolab.org>.
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

    // We cannot use the object we obtain from the model via get() in property bindings
    // according to ListModel documentation
    Component.onCompleted: subtitle.text = model.get(currentIndex).text
    onCurrentIndexChanged: subtitle.text = model.get(currentIndex).text
    width: parent.width

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
            height: Math.min(parent.height - (2 * margins), item.model.count * quranTheme.sizes.itemSmall + popupTitle.height + (2 * margins))

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
                        onClicked: {
                            item.currentIndex = index
                            popup.close()
                            popup.destroy(200)
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
