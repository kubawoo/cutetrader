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

    Connections {
        target: ConnectionManager
        function onConnectedChanged() {
            console.log("DEBUG: ConnectionManager.onConnectedChanged, connected =", ConnectionManager.connected)
            if (ConnectionManager.connected) {
                showConnectionDialog = false
                connectionDrawer.close()
            }
        }
    }

    Connections {
        target: root
        function onShowConnectionDialogChanged() {
            console.log("DEBUG: onShowConnectionDialogChanged, value =", showConnectionDialog)
            if (showConnectionDialog) {
                connectionDrawer.open()
                console.log("DEBUG: drawer.open() called")
            } else {
                connectionDrawer.close()
            }
        }
    }

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
                    console.log("DEBUG: Connect button clicked, connected =", ConnectionManager.connected)
                    if (ConnectionManager.connected) {
                        ConnectionManager.disconnectFromTws()
                    } else {
                        connectionDrawer.open()
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
        onOpened: {
            console.log("DEBUG: drawer opened")
            connectionDialog.resetForm()
        }
        onClosed: console.log("DEBUG: drawer closed")
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
}