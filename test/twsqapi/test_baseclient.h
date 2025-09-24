#ifndef TEST_TWSQAPI_BASECLIENT_H
#define TEST_TWSQAPI_BASECLIENT_H

#include <QTest>


class BaseClientTest : public QObject
{
    Q_OBJECT
private slots:
    void testHandshake();
    void testHandshakeOldVersion();
    void testHandshakeInvalidMsg();

};


#endif
