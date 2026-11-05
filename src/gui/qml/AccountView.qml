import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import cutetrader 1.0

Pane {
    id: root

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10
        spacing: 10

        RowLayout {
            Label {
                text: qsTr("Account Information")
                font.bold: true
                font.pixelSize: 16
            }
            Item { Layout.fillWidth: true }
            Button {
                text: qsTr("Refresh")
                onClicked: ConnectionManager.requestAccountData()
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView {
                id: listView
                anchors.fill: parent
                clip: true
                model: accountModel
                spacing: 0

                delegate: Item {
                    width: listView.width
                    height: 30
                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        Text {
                            text: model.key
                            width: 250
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: model.value
                            width: 200
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: model.currency
                            width: 120
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        height: 1
                        color: "#ddd"
                    }
                }

                header: Item {
                    width: listView.width
                    height: 35
                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        Text { text: qsTr("Key"); font.bold: true; width: 250; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Value"); font.bold: true; width: 200; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Currency"); font.bold: true; width: 120; verticalAlignment: Text.AlignVCenter }
                    }
                    Rectangle {
                        anchors.left: parent.left
                        anchors.right: parent.right
                        anchors.bottom: parent.bottom
                        height: 1
                        color: "#999"
                    }
                }
            }
        }

        Label {
            id: emptyLabel
            text: qsTr("Not connected or no account data available")
            color: "#888"
            visible: accountModel.count === 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    ListModel {
        id: accountModel
    }

    Connections {
        target: ConnectionManager
        function onAccountDataReceived(data) {
            accountModel.clear()
            for (var key in data) {
                var item = data[key]
                // Skip entries that don't have the expected {value, currency} structure
                if (typeof item !== 'object' || item === null || typeof item.value === 'undefined') {
                    continue
                }
                accountModel.append({
                    "key": key,
                    "value": item.value,
                    "currency": item.currency
                })
            }
        }
        function onConnectedChanged() {
            if (!ConnectionManager.connected) {
                accountModel.clear()
            }
        }
    }
}