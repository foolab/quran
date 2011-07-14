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

ListModel {
  Component.onCompleted: {
    // HACK: Seems the only way to catch the signal from a global object is this way
    _bookmarks.cleared.connect(clear);
    _bookmarks.bookmarkRemoved.connect(bookmarkRemoved);

    var bs = _bookmarks.bookmarks();

    for (var x = 0; x < bs.length; x++) {
      addBookmark(bs[x]);
    }
  }

  function removeBookmark(index) {
    _bookmarks.removeByIndex(index);
  }

  function bookmarkRemoved(b, index) {
    remove(index);
  }

  function addBookmark(b) {
    var sura = _bookmarks.sura(b);
    var aya = _bookmarks.aya(b);
    append({"bookmark": b, "sura": sura, "aya": aya});
  }
}
