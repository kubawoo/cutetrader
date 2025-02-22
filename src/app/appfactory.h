#ifndef APPFACTORY_H
#define APPFACTORY_H

#include <common.h>

class AppFactory: public common::IAppFactory
{
public:
    AppFactory();
    ~AppFactory();

    common::ITwsClient * createTwsClient();
    void setupDatabase();
};

#endif // APPFACTORY_H
