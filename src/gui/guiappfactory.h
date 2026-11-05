#ifndef GUI_APPFACTORY_H
#define GUI_APPFACTORY_H

#include <common.h>

class GuiAppFactory : public common::IAppFactory
{
public:
    GuiAppFactory();
    ~GuiAppFactory();

    common::ITwsClient * createTwsClient() override;
    void setupDatabase(const QString & dbName) override;
};

#endif // GUI_APPFACTORY_H