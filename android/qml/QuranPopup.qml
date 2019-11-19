// -*- qml -*-
/*
 * Copyright (c) 2011-2019 Mohammed Sameer <msameer@foolab.org>.
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
import QtQuick.Controls 2.4
import QuranAndroid 1.0

Popup {
    id: popup
    margins: quranTheme.sizes.marginMedium
    width: parent.width - (2 * margins)
    modal: true
    focus: true

    Overlay.modal: Rectangle {
        color: "#88000000"
    }

    enter: Transition {
        NumberAnimation {
            property: "scale"
            from: 0.0
            to: 1.0
            duration: quranTheme.animations.fast
        }
    }

    exit: Transition {
        NumberAnimation {
            property: "scale"
            from: 1.0
            to: 0.0
            duration: quranTheme.animations.fast * 2
        }
    }

    // We must assign KeyFilter to a property
    // because if we do not then it will be destroyed
    // contentItem is the default property and assigning anything to it will
    // cause already set children to be deleted.
    property var __filter: KeyFilter {
        enabled: popup.visible
        onBackTriggered: {
            popup.close()
        }
    }
}
