import QtQml 2.12
import QtQuick 2.12
import QtQuick.Controls 2.12

Page {
    title: qsTr("Home")

    //    Component.onCompleted: {
    //        tableView.forceLayout()
    //    }
    TableView {
        id: tableView
        anchors.fill: parent
        model: regModel
        reuseItems: true
        delegate: cellDelegate
        topMargin: columnsHeader.implicitHeight

        //        property var columnWidths: []
        //        columnWidthProvider: function (column) {
        //            return columnWidths[column]
        //        }
        Row {
            id: columnsHeader
            y: tableView.contentY
            z: 2
            Repeater {
                model: tableView.columns > 0 ? tableView.columns : 1
                Label {
                    width: tableView.columnWidths[index]
                    text: regModel.headerData(index, Qt.Horizontal)
                    padding: 3
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    background: Rectangle {
                        color: "#ff3333"
                    }
                }
            }
        }
    }

    Component {
        id: cellDelegate
        Rectangle {
            implicitWidth: textItem.implicitWidth + textItem.padding * 2
            implicitHeight: textItem.implicitHeight
            border.color: '#101010'
            MouseArea {
                anchors.fill: parent
            }
            Text {
                id: textItem
                anchors.fill: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: column === 1 ? Text.AlignHCenter : Text.AlignLeft
                padding: 10
                text: display
                elide: Text.ElideRight
                onTextChanged: {
                    //                    if (tableView.columnWidths[column] == null)
                    //                        tableView.columnWidths[column] = 10
                    //                    else if (tableView.columnWidths[column] < implicitWidth)
                    //                        tableView.columnWidths[column] = implicitWidth
                    //                    //                    }
                    //                    console.log(tableView.view, column, implicitWidth)
                    tableView.forceLayout()
                }
            }
        }
    }
}
