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

Column {
    id: keyboard

    width: parent.width

    signal enterClicked
    signal backspaceClicked
    signal keyClicked(string code)
    signal keyPressed(string code)
    signal keyReleased(string code)

    Row {
        width: parent.width
        height: quranTheme.characterKeySize

        CharacterKey { caption: "ض"; width: parent.width / 11 }
        CharacterKey { caption: "ص"; width: parent.width / 11 }
        CharacterKey { caption: "ث"; width: parent.width / 11 }
        CharacterKey { caption: "ق"; width: parent.width / 11 }
        CharacterKey { caption: "ف"; width: parent.width / 11 }
        CharacterKey { caption: "غ"; width: parent.width / 11 }
        CharacterKey { caption: "ع"; width: parent.width / 11 }
        CharacterKey { caption: "ه"; width: parent.width / 11 }
        CharacterKey { caption: "خ"; width: parent.width / 11 }
        CharacterKey { caption: "ح"; width: parent.width / 11 }
        CharacterKey { caption: "ج"; width: parent.width / 11 }
    }

    Row {
        width: parent.width
        height: quranTheme.characterKeySize

        CharacterKey { caption: "ش"; width: parent.width / 11 }
        CharacterKey { caption: "س"; width: parent.width / 11 }
        CharacterKey { caption: "ي"; width: parent.width / 11 }
        CharacterKey { caption: "ب"; width: parent.width / 11 }
        CharacterKey { caption: "ل"; width: parent.width / 11 }
        CharacterKey { caption: "ا"; width: parent.width / 11 }
        CharacterKey { caption: "ت"; width: parent.width / 11 }
        CharacterKey { caption: "ن"; width: parent.width / 11 }
        CharacterKey { caption: "م"; width: parent.width / 11 }
        CharacterKey { caption: "ك"; width: parent.width / 11 }
        CharacterKey { caption: "د"; width: parent.width / 11 }
    }

    Row {
        width: parent.width
        height: quranTheme.characterKeySize

        CharacterKey { caption: "ء"; width: parent.width / 11 }
        CharacterKey { caption: "ذ"; width: parent.width / 11 }
        CharacterKey { caption: "ر"; width: parent.width / 11 }
        CharacterKey { caption: "ى"; width: parent.width / 11 }
        CharacterKey { caption: "ة"; width: parent.width / 11 }
        CharacterKey { caption: "و"; width: parent.width / 11 }
        CharacterKey { caption: "ز"; width: parent.width / 11 }
        CharacterKey { caption: "ظ"; width: parent.width / 11 }
        CharacterKey { caption: "ط"; width: parent.width / 11 }
        ToolButton { icon.source: quranTheme.keyboardBackspace; width: 2 * (parent.width / 11); height: parent.width / 11; onClicked: keyboard.backspaceClicked() }
    }

    Row {
        width: parent.width
        height: quranTheme.characterKeySize

        CharacterKey { caption: "إ"; width: parent.width / 11 }
        CharacterKey { caption: "أ"; width: parent.width / 11 }
        CharacterKey { caption: "آ"; width: parent.width / 11 }
        CharacterKey { caption: "ئ"; width: parent.width / 11 }
        CharacterKey { caption: "ؤ"; width: parent.width / 11 }
        CharacterKey { caption: " "; width: 4 * (parent.width / 11) }
        ToolButton { icon.source: quranTheme.keyboardEnter; width: 2 * (parent.width / 11); height: parent.width / 11; onClicked: keyboard.enterClicked() }
    }
}
