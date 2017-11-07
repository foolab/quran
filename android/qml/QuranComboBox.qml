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

    property list<MenuAction> actions
    property alias label: title.text
    property int currentIndex //: combo.currentIndex

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

            text: item.actions[item.currentIndex].text
            color: parent.highlighted ? quranTheme.colors.secondaryHighlight : quranTheme.colors.secondary
        }
    }

    Component {
        id: dialog

        Popup {
            id: popup
            parent: stack.currentItem
            x: (parent.width - width) / 2
            y: (parent.height - height) / 2
            width: parent.width - (2 * quranTheme.sizes.marginMedium)
            height: Math.min(parent.height - (2 * quranTheme.sizes.marginMedium), item.actions.length * quranTheme.sizes.itemSmall + popupTitle.height + quranTheme.sizes.spacing)
            modal: true
            focus: true

            enter: Transition {
                NumberAnimation { property: "opacity"; from: 0.0; to: 1.0; duration: 100 }
            }

            exit: Transition {
                NumberAnimation { property: "opacity"; from: 1.0; to: 0.0; duration: 100 }
            }

            contentItem: Column {
                width: parent.width
                height: parent.height
                spacing: quranTheme.sizes.spacing
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
                    width: parent.width
                    height: parent.height - quranTheme.sizes.spacing - popupTitle.height
                    clip: true
                    model: item.actions

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
                        }

                        QuranLabel {
                            anchors {
                                fill: parent
                                margins: quranTheme.sizes.marginSmall
                            }
                            text: modelData.text
                            verticalAlignment: Text.AlignVCenter
                            color: parent._color(index)
                        }
                    }
                }
            }
        }
    }
}
