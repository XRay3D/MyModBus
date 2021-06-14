import QtQuick 2.12
import QtQuick.Controls 2.5
import Qt.labs.settings 1.1

ApplicationWindow {
    id: window
    width: 1280
    height: 720
    visible: true
    title: qsTr("Stack")

    Settings {
        property alias x: window.x
        property alias y: window.y
        property alias width: window.width
        property alias height: window.height
    }

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            text: stackView.currentItem.title
            anchors.centerIn: parent
        }
    }

    onClosing: Qt.quit()

    Drawer {
        id: drawer
        width: window.width * 0.66
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Page 1")
                width: parent.width
                onClicked: {
                    stackView.push("Page1Form.ui.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("Page 2")
                width: parent.width
                onClicked: {
                    stackView.push("Page2Form.ui.qml")
                    drawer.close()
                }
            }
            GroupBox {
                title: 'Acces'
                Column {
                    Repeater {
                        anchors.fill: parent
                        model: regModel.acces
                        RadioButton {
                            text: regModel.acces[index]
                        }
                    }
                }
            }
            GroupBox {
                title: 'Type'
                Column {
                    Repeater {
                        anchors.fill: parent
                        model: regModel.type
                        RadioButton {
                            text: regModel.type[index]
                        }
                    }
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeForm.qml"
        anchors.fill: parent
    }
}
