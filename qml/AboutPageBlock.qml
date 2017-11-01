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

import QtQuick 2.0

QuranBackgroundItem {
    property alias title: titleLabel.text
    property alias text: textLabel.text

    height: titleLabel.height + textLabel.height
    width: parent.width

    QuranLabel {
        id: titleLabel
        font.family: quranTheme.fontFamilyHeading
        font.bold: true
        color: highlighted ? quranTheme.colors.secondaryHighlight : quranTheme.colors.secondary
    }

    QuranLabel {
        id: textLabel
        anchors.top: titleLabel.bottom
        width: parent.width
        textFormat: Text.RichText
        color: highlighted ? quranTheme.colors.primaryHighlight : quranTheme.colors.primary
    }
}
