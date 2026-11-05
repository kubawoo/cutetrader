import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import cutetrader 1.0

RowLayout {
    id: root
    spacing: 10

    Rectangle {
        id: statusIndicator
        width: 10
        height: 10
        radius: 5
        color: ConnectionManager.connected ? "#4CAF50" : "#F44336"
        Layout.alignment: Qt.AlignVCenter
    }

    Label {
        id: statusLabel
        text: ConnectionManager.status
        font.pixelSize: 12
        Layout.alignment: Qt.AlignVCenter
    }

    Label {
        id: accountLabel
        text: ConnectionManager.accountId ? qsTr("Account: %1").arg(ConnectionManager.accountId) : qsTr("No account")
        font.pixelSize: 12
        color: "#888"
        Layout.alignment: Qt.AlignVCenter
    }
}