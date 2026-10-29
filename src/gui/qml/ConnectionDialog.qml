import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import cutetrader 1.0

Pane {
    id: root
    signal accepted()

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 20
        spacing: 15

        Label {
            text: qsTr("Connect to TWS / IB Gateway")
            font.bold: true
            font.pixelSize: 18
            Layout.alignment: Qt.AlignHCenter
        }

        Frame {
            Layout.fillWidth: true
            ColumnLayout {
                anchors.fill: parent
                anchors.margins: 15
                spacing: 10

                RowLayout {
                    Label { text: qsTr("Mode:"); Layout.preferredWidth: 100 }
                    ComboBox {
                        id: modeCombo
                        Layout.fillWidth: true
                        model: ["Live (TWS/Gateway)", "Mocked (Offline)"]
                        currentIndex: ConnectionManager.useMock ? 1 : 0
                        onCurrentIndexChanged: ConnectionManager.useMock = (currentIndex === 1)
                    }
                }

                RowLayout {
                    Label { text: qsTr("Host:"); Layout.preferredWidth: 100 }
                    TextField {
                        id: hostField
                        Layout.fillWidth: true
                        text: ConnectionManager.host
                        placeholderText: "localhost"
                        onAccepted: ConnectionManager.host = text
                    }
                }

                RowLayout {
                    Label { text: qsTr("Port:"); Layout.preferredWidth: 100 }
                    SpinBox {
                        id: portSpin
                        Layout.fillWidth: true
                        value: ConnectionManager.port
                        from: 1
                        to: 65535
                        onValueChanged: ConnectionManager.port = value
                    }
                }

                RowLayout {
                    Label { text: qsTr("Client ID:"); Layout.preferredWidth: 100 }
                    SpinBox {
                        id: clientIdSpin
                        Layout.fillWidth: true
                        value: ConnectionManager.clientId
                        from: 0
                        to: 100
                        onValueChanged: ConnectionManager.clientId = value
                    }
                }

                RowLayout {
                    Label { text: qsTr("Account ID:"); Layout.preferredWidth: 100 }
                    TextField {
                        id: accountField
                        Layout.fillWidth: true
                        text: ConnectionManager.accountId
                        placeholderText: qsTr("Optional - auto-detect if empty")
                        onAccepted: ConnectionManager.accountId = text
                    }
                }
            }
        }

        Item { Layout.fillHeight: true }

        RowLayout {
            Layout.fillWidth: true
            Item { Layout.fillWidth: true }
            Button {
                text: qsTr("Cancel")
                onClicked: root.resetForm()
            }
            Button {
                text: qsTr("Connect")
                highlighted: true
                onClicked: {
                    ConnectionManager.connectToTws()
                    accepted()
                }
            }
        }
    }

    function resetForm() {
        hostField.text = ConnectionManager.host
        portSpin.value = ConnectionManager.port
        clientIdSpin.value = ConnectionManager.clientId
        accountField.text = ConnectionManager.accountId
        modeCombo.currentIndex = ConnectionManager.useMock ? 1 : 0
    }
}