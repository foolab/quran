// -*- qml-mode -*-
import QtQuick 1.0

ListModel {
        id: model

        Component.onCompleted: {
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
