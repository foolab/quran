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
import com.nokia.extras 1.0

PageStackWindow {
  id: window
  property int textType: _settings.textType;

  InfoBanner {
    id: favoriteAdded
    text: qsTr("Favorite added");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50

    Component.onCompleted: {
      _bookmarks.bookmarkAdded.connect(show);
    }

  }

  InfoBanner {
    id: favoriteRemoved
    text: qsTr("Favorite removed");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50

    Component.onCompleted: {
      _bookmarks.bookmarkRemoved.connect(show);
    }

  }

  InfoBanner {
    id: favoritesCleared
    text: qsTr("Favorites cleared");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50

    Component.onCompleted: {
      _bookmarks.cleared.connect(show);
    }
  }

  InfoBanner {
    id: maxFontSize
    text: qsTr("Maximum font size reached");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50
  }

  InfoBanner {
    id: minFontSize
    text: qsTr("Minimum font size reached");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50
  }

  InfoBanner {
    id: fontError
    text: qsTr("Failed to load application font");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50
  }

  InfoBanner {
    id: textError
    text: qsTr("Failed to load the text");

    // HACK because the banner appears underneath the statusbar :|
    topMargin: 50
  }

  onTextTypeChanged: {
    if (!_data.setText(textType)) {
      textError.show();
      // TODO: reset settings back
    }
  }

  Component.onCompleted: {
    if (!_settings.fontLoaded) {
      fontError.show();
    }

    if (!_data.setText(textType)) {
      pageStack.push(Qt.createComponent("ErrorPage.qml"));
    }
    else {
      pageStack.push(Qt.createComponent("QuranPage.qml"), {}, true);
    }
  }
}
