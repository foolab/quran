// -*- qml -*-
import QtQuick 1.0

ListModel {
        id: model

        Component.onCompleted: {
            var bs = bookmarks.bookmarks();

            for (var x = 0; x < bs.length; x++) {
                addBookmark(bs[x]);
            }
        }

        function removeBookmark(index) {
                bookmarks.removeByIndex(index);
        }

        function bookmarkRemoved(b, index) {
                remove(index);
        }

        function addBookmark(b) {
                var sura = bookmarks.sura(b);
                var aya = bookmarks.aya(b);
                append({"bookmark": b, "sura": sura, "aya": aya});
        }
}
