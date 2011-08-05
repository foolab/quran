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
import Quran 1.0

Page {
  id: quranpage
  orientationLock: PageOrientation.LockPortrait

  function doScrollTo(sura, aya) {
    if (view.currentItem) {
      view.currentItem.scrollTo(sura, aya);
    }
  }

  function createPage(name, useSignal) {
    var component = Qt.createComponent(name + ".qml");
    if (component.status == Component.Ready) {
      var p = pageStack.push(component);
      if (useSignal) {
        p.scrollTo.connect(doScrollTo);
      }
    }
  }

  // We cannot use "anchors.fill: parent" here because we set the ListView layout direction
  // to RTL and both will not play nice :|
  width: screen.displayHeight

  // We have to anchor because if we set height instead we will "overflow" the page and
  // end up rendering below the toolbar.
  anchors.bottom: parent.bottom

  tools: toolBar

  Menu {
    id: menu
    MenuLayout {
      MenuItem {
        text: qsTr("Settings")
        onClicked: createPage("SettingsPage", false);
      }

      MenuItem {
        text: qsTr("About")
        onClicked: createPage("AboutPage", false);
      }
    }
  }

  AddToFavoritesMenu {
    id: addFavorite
  }

  RemoveFromFavoritesMenu {
    id: rmFavorite
  }

  ToolBarLayout {
    id: toolBar

    ToolIcon {
      id: favoritesIcon
      iconId: "toolbar-favorite-mark"
      onClicked: {
        if (_bookmarks.empty) {
          createPage("BookmarksPageEmpty", false);
        }
        else {
          createPage("BookmarksPage", true);
        }
      }
    }

    ToolIcon {
      id: listIcon
      iconId: "toolbar-list"
      onClicked: createPage("IndexPage", true);
    }

    ToolIcon {
      id: menuIcon
      iconId: "toolbar-view-menu"
      onClicked: (menu.status == DialogStatus.Closed) ? menu.open() : menu.close()
    }
  }

  Component {
    id: viewView

    Rectangle {
      width: view.width
      height: view.height
      function scrollTo(sura, aya) {
        content.scrollTo(sura, aya);
      }

      function setY(yp) {
        animation.run(yp);
      }

      QuranPageSuraList {
        id: name
        suras: _data.surasForPage(index)
	anchors.right: part.left
	anchors.left: parent.left
        anchors.leftMargin: 20
      }

      Label {
        id: part
        text: _data.partName(index);
        anchors.right: parent.right
        anchors.rightMargin: 20
      }

      Flickable {
        id: flick
        flickableDirection: Flickable.VerticalFlick
        anchors.right: parent.right
        anchors.left: parent.left
        anchors.top: name.bottom
	width: parent.width
        height: parent.height - part.height - number.height
        contentHeight: content.height
	clip: true
        interactive: !pinch.pinch.active

        onContentYChanged: {
          _settings.y = contentY > 0 ? contentY : 0;
        }

        NumberAnimation {
          id: animation
          target: flick
          from: flick.contentY
          easing.type: Easing.InOutQuad
          property: "contentY"

          function run(t) {
            complete();
            from = flick.contentY;
            to = t;
            start();
          }
        }

        QuranView {
          id: content

          // HACK: Seems the only way to listen to _settings.numberFormat
          // and _settings.textType is to alias them to properties!
          property int format: _settings.numberFormat
          property int textType: _settings.textType

          font.pointSize: _settings.fontSize
          font.family: _settings.fontFamily
          anchors.right: parent.right
          anchors.left: parent.left
          anchors.top: parent.top
          margin: 20
          page: index
          dataProvider: _data
	  bookmarks: _bookmarks
          formatter: _formatter
          highlightColor: _settings.highlightColor

          onFormatChanged: populate();
          onTextTypeChanged: populate();

          onLineVisibilityRequested: {
            if (upper >= flick.contentY && lower <= flick.contentY + flick.height) {
              // Nothing.
              return;
            }
            else if (lower <= flick.contentY + flick.height) {
              // Topmost part is not visible.
              // We will scroll anyway and make it visible.
              animation.run(upper);
              return;
            }

            if (lower - upper > flick.height) {
              // The line will not fit no matter what we do.
              // Just show the upper part.
              animation.run(upper);
              return;
            }

            // Our line will fit the view. We need to scroll until the bottommost part
            // is just visible.
            var part = upper + (lower - (upper + flick.height));
            animation.run(part);
          }

          PinchArea {
            id: pinch

            anchors.fill: parent
            anchors.right: parent.right
            anchors.left: parent.left
            anchors.top: parent.top
            anchors.bottom: parent.bottom
            width: parent.width
            height: parent.height
            pinch.target: parent
            pinch.minimumScale: _settings.minFontSize / _settings.fontSize
            pinch.maximumScale: _settings.maxFontSize / _settings.fontSize

            onPinchUpdated: {
              var size = content.scale * _settings.fontSize;

              if (pinch.scale < 1.0 && size <= _settings.minFontSize) {
                minFontSize.show();
              }
              else if (pinch.scale > 1.0 && size >= _settings.maxFontSize) {
                maxFontSize.show();
              }
            }

            onPinchFinished: {
              var len = pinch.startCenter.y - flick.contentY;
              _settings.fontSize = content.scale * _settings.fontSize;
              content.scale = 1;
              animation.to = Math.min(Math.max(len), content.height - flick.height)
            }
          }

          MouseArea {
            anchors.fill: parent
            enabled: !pinch.pinch.active
            onClicked: content.mouseClicked(mouse.x, mouse.y);
            onPressAndHold: {
              var b = content.bookmarkId(mouse.x, mouse.y);
              if (b == undefined) {
                return;
              }

              if (_bookmarks.isBookmarked(b)) {
                rmFavorite.run(content.textForPosition(mouse.x, mouse.y), b);
              }
              else {
                addFavorite.run(content.textForPosition(mouse.x, mouse.y), b);
              }
            }
          }
        }
      }

      NumberLabel {
        id: number
        anchors.top: flick.bottom
	anchors.right: parent.right
	anchors.left: parent.left
        number: index + 1
	horizontalAlignment: Text.AlignHCenter
      }
    }
  }

  ListView {
    layoutDirection: Qt.RightToLeft

    id: view
    highlightMoveDuration: 10
    flickableDirection: Flickable.HorizontalFlick
    orientation: ListView.Horizontal
    snapMode: ListView.SnapOneItem
    highlightRangeMode: ListView.StrictlyEnforceRange
    anchors.fill: parent
    model: _data.pageCount()
    currentIndex: _settings.pageNumber
    delegate: viewView
    cacheBuffer: 0
    onFlickEnded: _settings.pageNumber = currentIndex

    // TODO: couldn't find a better way :|
    property bool __set: false;
    onCurrentItemChanged: {
      if (currentItem && !__set) {
        currentItem.setY(_settings.y);
        __set = true;
      }
      else if (currentItem && __set) {
        _settings.y = 0;
      }
    }
  }
}
