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

Rectangle {
    property bool downloading: false
    property bool isInstalled: false
    property bool error: false
    property bool _dimmed: !(isInstalled || downloading || error)
    color: downloading ? quranTheme.statusDownloading : error ? quranTheme.statusError : quranTheme.statusInstalled
    width: quranTheme.itemSizeSmall / 2
    height: width
    radius: width / 3
    opacity: _dimmed ? 0.3 : 1.0
    smooth: true
}
