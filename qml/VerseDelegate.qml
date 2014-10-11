// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

ContextMenuLabel {
        id: item
        contentHeight: label.height

        onClicked: {
                pagePosition.setPosition(chapter, verse)
                pageStack.pop()
        }

        content: QuranLabel {
                id: label
                width: parent.width

                NumberFormatter {
                        id: formatter
                        format: settings.numberFormat
                        number: verse + 1
                }

                font.family: settings.fontFamily
                text: qsTr("(%1) %2").arg(formatter.formattedNumber).arg(_data.text(chapter, verse))
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignRight
                color: theme.primaryColor
        }
}
