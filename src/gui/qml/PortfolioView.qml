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
                text: qsTr("Portfolio Positions")
                font.bold: true
                font.pixelSize: 16
            }
            Item { Layout.fillWidth: true }
            Button {
                text: qsTr("Refresh")
                onClicked: ConnectionManager.requestPortfolio()
            }
        }

        Frame {
            Layout.fillWidth: true
            Layout.fillHeight: true
            ListView {
                id: listView
                anchors.fill: parent
                clip: true
                model: portfolioModel
                spacing: 0

                delegate: Item {
                    width: listView.width
                    height: 30
                    Row {
                        anchors.fill: parent
                        anchors.margins: 5
                        Text { text: model.symbol; width: 100; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.type; width: 80; elide: Text.ElideRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.position; width: 80; elide: Text.ElideRight; horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.marketPrice; width: 100; elide: Text.ElideRight; horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.marketValue; width: 100; elide: Text.ElideRight; horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter }
                        Text { text: model.averageCost; width: 100; elide: Text.ElideRight; horizontalAlignment: Text.AlignRight; verticalAlignment: Text.AlignVCenter }
                        Text {
                            text: typeof model.unrealizedPNL === 'number' ? model.unrealizedPNL.toLocaleString(undefined, {minimumFractionDigits: 2, maximumFractionDigits: 2}) : model.unrealizedPNL
                            color: typeof model.unrealizedPNL === 'number' && model.unrealizedPNL >= 0 ? "#4CAF50" : (typeof model.unrealizedPNL === 'number' ? "#F44336" : "#000")
                            font.bold: true
                            width: 100
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
                            verticalAlignment: Text.AlignVCenter
                        }
                        Text {
                            text: typeof model.realizedPNL === 'number' ? model.realizedPNL.toLocaleString(undefined, {minimumFractionDigits: 2, maximumFractionDigits: 2}) : model.realizedPNL
                            width: 100
                            elide: Text.ElideRight
                            horizontalAlignment: Text.AlignRight
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
                        Text { text: qsTr("Symbol"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Type"); font.bold: true; width: 80; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Position"); font.bold: true; width: 80; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Mkt Price"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Mkt Value"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Avg Cost"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Unrealized P&L"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
                        Text { text: qsTr("Realized P&L"); font.bold: true; width: 100; verticalAlignment: Text.AlignVCenter }
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
            text: qsTr("No portfolio positions")
            color: "#888"
            visible: portfolioModel.count === 0
            Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    ListModel {
        id: portfolioModel
    }

    QtObject {
        id: helpers
        function securityTypeToString(type) {
            switch (type) {
            case 0: return "UNKNOWN"
            case 1: return "STOCK"
            case 2: return "OPTION"
            case 3: return "FUTURE"
            case 4: return "FUT_OPT"
            case 5: return "INDEX"
            default: return "OTHER"
            }
        }
    }

    Connections {
        target: ConnectionManager
        function onPortfolioReceived(positions) {
            portfolioModel.clear()
            for (var i = 0; i < positions.length; ++i) {
                var pos = positions[i]
                portfolioModel.append({
                    "symbol": pos.symbol,
                    "type": helpers.securityTypeToString(pos.securityType),
                    "position": pos.position,
                    "marketPrice": pos.marketPrice,
                    "marketValue": pos.marketValue,
                    "averageCost": pos.averageCost,
                    "unrealizedPNL": pos.unrealizedPNL,
                    "realizedPNL": pos.realizedPNL
                })
            }
        }
        function onConnectedChanged() {
            if (!ConnectionManager.connected) {
                portfolioModel.clear()
            }
        }
    }
}