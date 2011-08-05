/*
 * Copyright (c) 2011 Mohammed Sameer <msameer@foolab.org>. All rights reserved.
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

import QtQuick 1.1
import com.nokia.meego 1.0

Button {
  property int page: _settings.pageNumber
  // HACK: prefix with underscore because sura and aya are used in BookmarksModel
  property int _sura: -1
  property int _aya: -1
  property bool __clicked: false

  onPageChanged: {
    if (__clicked == false) {
      return;
    }

    if (_sura != -1 && _aya != -1) {
      pageStack.currentPage.scrollTo(_sura, _aya);

      _sura = -1;
      _aya = -1;

      pageStack.pop();
    }
  }

  onClicked: {
    __clicked = true;
    var p = _data.pageNumberForSuraAndAya(_sura, _aya);
    if (p == page) {
      pageStack.currentPage.scrollTo(_sura, _aya);

      _sura = -1;
      _aya = -1;

      pageStack.pop();
    }
    else {
      _settings.pageNumber = p;
    }
  }
}
