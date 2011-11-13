// -*- qml -*-
import QtQuick 1.0

QtObject {
        id: manager
        signal reloaded

        property QtObject theme: null
        property Component __component: Qt.createComponent("Theme.qml");

        function loadTheme(id) {
                if (theme) {
                        theme.destroy();
                }

                _theme.id = id;

                theme = __component.createObject(manager);

                reloaded();
        }
}
