#include "test_baseclient.h"
#include "testclient.h"

void BaseClientTest::testHandshake()
{
    QByteArray inBytes, outBytes;

    TestApiClient client;
    client.connect(&inBytes, &outBytes);

    QCOMPARE(inBytes,
             QByteArray(""));
    QCOMPARE(outBytes,
             QByteArray("API\0\0\0\0\x12v198..198 +PACEAPI", 26));

    inBytes.append("\x0\x0\x0\x35", 4);
    inBytes.append("198\00020250224 08:25:56 Central European Standard Time\0", 53);
    client.read();

    QVERIFY(client.handshakeDone());
    QCOMPARE(198, client.getApiVersion());
}

void BaseClientTest::testHandshakeOldVersion()
{
    QByteArray inBytes, outBytes;

    TestApiClient client;
    client.connect(&inBytes, &outBytes);

    QCOMPARE(inBytes,
             QByteArray(""));
    QCOMPARE(outBytes,
             QByteArray("API\0\0\0\0\x12v198..198 +PACEAPI", 26));

    inBytes.append("\x0\x0\x0\x35", 4);
    inBytes.append("197\00020250224 08:25:56 Central European Standard Time\0", 53);
    client.read();

    QVERIFY(!client.handshakeDone());
    QCOMPARE(197, client.getApiVersion());
}

void BaseClientTest::testHandshakeInvalidMsg()
{
    QByteArray inBytes, outBytes;

    TestApiClient client;
    client.connect(&inBytes, &outBytes);

    QCOMPARE(inBytes,
             QByteArray(""));
    QCOMPARE(outBytes,
             QByteArray("API\0\0\0\0\x12v198..198 +PACEAPI", 26));

    inBytes.append("\x0\x0\x0\x35", 4);
    inBytes.append("198\000\00020250224 08:25:56 Central European Standard Time", 53);
    client.read();

    QVERIFY(!client.handshakeDone());
}
