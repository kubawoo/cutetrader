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
                text: qsTr("Open Orders")
                font.bold: true
                font.pixelSize: 16
            }
            Item { Layout.fillWidth: true }
            Button {
                text: qsTr("Refresh")
                onClicked: ConnectionManager.requestOpenOrders()
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView {
                id: listView
                anchors.fill: parent
                clip: true
                model: ordersModel
                spacing: 0

                delegate: Item {
                    width: listView.width
                    height: 30
                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        Text { text: model.orderId; width: 80; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.symbol; width: 100; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.action; width: 80; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: helpers.formatNumber(model.quantity); width: 80; elide: Text.ElideRight; horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.orderType; width: 100; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                        Text {
                            text: helpers.formatNumber(model.limitPrice)
                            width: 100
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: model.status
                            color: model.status === "Filled" ? "#4CAF50" :
                                   model.status === "Cancelled" ? "#F44336" :
                                   model.status === "Pending" ? "#FF9800" : "#2196F3"
                            font.bold: true
                            width: 100
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignHCenter
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
                        Text { text: qsTr("Order ID"); font.bold: true; width: 80; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Symbol"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Action"); font.bold: true; width: 80; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Quantity"); font.bold: true; width: 80; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Type"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Limit Price"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Status"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
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
            text: qsTr("No open orders")
            color: "#888"
            visible: ordersModel.count === 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    ListModel {
        id: ordersModel
    }

    QtObject {
        id: helpers
        function formatNumber(value) {
            var num = Number(value)
            if (isNaN(num)) {
                return value
            }
            // Qt's JS engine ignores the options of toLocaleString(), so use toFixed().
            return num.toFixed(2)
        }
    }

    Connections {
        target: ConnectionManager
        function onOpenOrdersReceived(orders) {
            ordersModel.clear()
            for (var i = 0; i < orders.length; ++i) {
                var order = orders[i]
                ordersModel.append({
                    "orderId": order.orderId,
                    "symbol": order.symbol,
                    "action": order.action,
                    "quantity": order.totalQuantity,
                    "orderType": order.orderType,
                    "limitPrice": order.lmtPrice,
                    "status": order.status
                })
            }
        }
        function onConnectedChanged() {
            if (!ConnectionManager.connected) {
                ordersModel.clear()
            }
        }
    }
}