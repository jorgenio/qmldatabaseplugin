import QtQuick 2.0
import me.qtquick.Database 0.1

Item {
    id: root
    width: 400
    height: 300

    Database {
        id: db
        connectionName: 'examples/database.qml'
        type: "QSQLITE"
        databaseName: "ventas"
        hostName:"localhost"
        userName:"postgres"
        password:"uno"

        TableModel {
            id: table
            tableName: 'Chat'
            primaryKey: 'key'
            property int key
            property string value
        }

        SqlModel {
            id: select
            query: "SELECT COUNT(key) as keys FROM Chat WHERE value LIKE ?"
            params: ['%Qt%']
        }
    }

    Rectangle {
        id: header
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.margins: 2
        height: field.font.pixelSize * 1.5

        border.color: 'gray'
        border.width: 1

        TextInput {
            id: field
            anchors.fill: parent
            anchors.margins: 2
            focus: true


            Keys.onReturnPressed: {
                select.select = false
                table.insert({'value': field.text})
                field.text = ''
                select.select = true
            }
        }
    }

    Flickable {
        anchors.top: header.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.bottom: footer.top
        contentWidth: width
        contentHeight: contents.height
        Grid {
            id: contents
            width: parent.width
            flow: Grid.TopToBottom
            rows: table.count + 1
            Text {
                width: 100
                text: 'key'
            }

            Repeater {
                model: table
                Text {
                    text: model.key
                }
            }
            Text {
                text: 'value'
            }

            Repeater {
                model: table
                Text {
                    text: model.value
                    MouseArea {
                        anchors.fill: parent
                        onClicked: {
                            select.select = false
                            table.remove({'key': model.key})
                            select.select = true
                        }
                    }
                }
            }
        }
    }
    Text {
        id: footer
        anchors.bottom: parent.bottom
        anchors.left: parent.left
        anchors.right: parent.right
        text: 'the number of values that contains "Qt" is %1.'.arg(select.count > 0 ? select.get(0).keys : '0')
    }

}
