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

MouseArea {
    property bool shown: false
    property variant textSupplier

    width: parent.width
    height: label.height

    visible: opacity > 0
    opacity: label.text != "" && ((shown && settings.translationsHidden) || !settings.translationsHidden) ? 1 : 0

    Behavior on opacity {
        NumberAnimation { duration: 100 }
    }

    onClicked: {
        if (settings.translationsHidden) {
            shown = !shown
        }
    }

    BorderLabel {
        id: label
        borderColor: quranTheme.quranColors.translationBorder
        backgroundColor: quranTheme.quranColors.translationBackground
        textColor: quranTheme.quranColors.translation
        text: textSupplier.secondaryText
        font.pixelSize: settings.translationFontSize
    }
}
