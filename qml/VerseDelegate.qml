// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

ContextMenuLabel {
        id: item
        property int _chapter
        property int _verse

        contentHeight: label.height

        onClicked: {
                pagePosition.setPosition(chapter, verse)
                pageStack.pop()
        }

        content: QuranLabel {
                id: label
                width: parent.width

                TextSupplier {
                        id: supplier
                        chapter: _chapter
                        verse: _verse
                        data: _data
                }

                NumberFormatter {
                        id: formatter
                        format: settings.numberFormat
                        number: verse + 1
                }

                font.family: settings.fontFamily
                text: qsTr("(%1) %2").arg(formatter.formattedNumber).arg(supplier.primaryText)
                wrapMode: Text.WordWrap
                horizontalAlignment: Text.AlignRight
                color: theme.primaryColor
        }
}
