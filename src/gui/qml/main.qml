import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtQuick.Window
import cutetrader 1.0

ApplicationWindow {
    id: root
    width: 1200
    height: 800
    minimumWidth: 800
    minimumHeight: 600
    visible: true
    title: "cuteTrader - IB TWS Client"

    property bool showConnectionDialog: !ConnectionManager.connected

    header: ToolBar {
        RowLayout {
            anchors.fill: parent
            Label {
                text: "cuteTrader"
                font.bold: true
                font.pixelSize: 18
                Layout.alignment: Qt.AlignVCenter
                Layout.leftMargin: 10
            }
            Item { Layout.fillWidth: true }
            StatusBar {
                Layout.alignment: Qt.AlignVCenter
                Layout.rightMargin: 10
            }
            Button {
                id: connectBtn
                text: ConnectionManager.connected ? "Disconnect" : "Connect"
                enabled: !ConnectionManager.connected || ConnectionManager.status !== "Connecting..."
                onClicked: {
                    if (ConnectionManager.connected) {
                        ConnectionManager.disconnectFromTws()
                    } else {
                        showConnectionDialog = true
                    }
                }
                Layout.rightMargin: 10
                Layout.alignment: Qt.AlignVCenter
            }
        }
    }

    Drawer {
        id: connectionDrawer
        width: Math.min(400, root.width * 0.9)
        height: root.height
        dragMargin: 0
        onOpened: connectionDialog.resetForm()
        ConnectionDialog {
            id: connectionDialog
            anchors.fill: parent
            onAccepted: connectionDrawer.close()
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        AccountView { }
        PortfolioView { }
        OrdersView { }
    }

    footer: TabBar {
        id: tabBar
        TabButton { text: qsTr("Account") }
        TabButton { text: qsTr("Portfolio") }
        TabButton { text: qsTr("Orders") }
    }

    Connections {
        target: ConnectionManager
        function onConnectedChanged() {
            if (ConnectionManager.connected) {
                showConnectionDialog = false
                connectionDrawer.close()
            }
        }
    }
}