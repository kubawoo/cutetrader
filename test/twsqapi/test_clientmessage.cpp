#include "test_clientmessage.h"
#include <twsqapi/clientmessage.h>


void ClientMessageTest::testHandshakeMessage()
{
    auto msg = twsqapi::HandshakeClientMessage("+PACEAPI");
    QCOMPARE(msg.encode(),
             QByteArray("API\x00\x00\x00\x00\x12v100..198 +PACEAPI", 26));
}


