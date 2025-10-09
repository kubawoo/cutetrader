#include "test_clientmessage.h"
#include <twsqapi/clientmessage.h>


void ClientMessageTest::testHandshakeMessage()
{
    auto msg = twsqapi::HandshakeClientMessage("+PACEAPI");
    QCOMPARE(msg.encode(),
             QByteArray("API\x00\x00\x00\x00\x12v198..198 +PACEAPI", 26));
}

void ClientMessageTest::testStartApiClientMessage()
{
    auto msg = twsqapi::StartApiClientMessage(22);
    QCOMPARE(msg.encode(),
             QByteArray("\x00\x00\x00\x09"
                        "71\x00"
                        "2\x00"
                        "22\x00\x00",
                        13));
}

void ClientMessageTest::testStartAccountUpdatesClientMessage()
{
    auto msg = twsqapi::StartAccountUpdatesClientMessage("U12345678");
    QCOMPARE(msg.encode(),
             QByteArray("\x00\x00\x00\x10"
                        "6\x00"
                        "2\x00"
                        "1\x00"
                        "U12345678\x00",
                        20));
}

void ClientMessageTest::testRequestCurrentTimeClientMessage()
{
    auto msg = twsqapi::RequestCurrentTimeClientMessage();
    QCOMPARE(msg.encode(),
             QByteArray("\x00\x00\x00\x05"
                        "49\x00"
                        "1\x00",
                        9));
}

void ClientMessageTest::testRequestMatchingSymbolsClientMessage()
{
    auto msg = twsqapi::RequestMatchingSymbolsClientMessage(3, "AAPL");
    QCOMPARE(msg.encode(),
             QByteArray("\x00\x00\x00\x0A"
                        "81\x00"
                        "3\x00"
                        "AAPL\x00",
                        14));
}

void ClientMessageTest::testRequestContractDetailsClientMessage()
{
    auto msg = twsqapi::RequestContractDetailsClientMessage(3, 2345);
    QCOMPARE(msg.encode(),
             QByteArray("\x00\x00\x00\x21"
                        "9\x00"
                        "8\x00"
                        "3\x00"
                        "2345\x00"
                        "\x00"
                        "\x00"
                        "\x00"
                        "0\x00"
                        "\x00"
                        "\x00"
                        "SMART\x00"
                        "\x00"
                        "\x00"
                        "\x00"
                        "\x00"
                        "0\x00"
                        "\x00"
                        "\x00"
                        "\x00",
                        37));
}
