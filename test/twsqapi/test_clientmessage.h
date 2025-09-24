#ifndef TEST_TWSQAPI_CLIENTMESSAGE_H
#define TEST_TWSQAPI_CLIENTMESSAGE_H

#include <QTest>


class ClientMessageTest : public QObject
{
    Q_OBJECT
private slots:
    void testHandshakeMessage();

};


#endif
