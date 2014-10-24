// -*- qml -*-
import QtQuick 2.0
import Quran 1.0

ListDelegate {
        id: item
        property int _chapter
        property int _verse

        onClicked: {
                pagePosition.setPosition(chapter, verse)
                popPage()
        }

        QuranLabel {
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
                horizontalAlignment: Text.AlignRight
                color: quranTheme.primaryColor
        }
}
