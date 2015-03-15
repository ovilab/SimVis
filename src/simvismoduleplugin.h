#ifndef SIMVISMODULEPLUGIN_H
#define SIMVISMODULEPLUGIN_H

#include <QQmlExtensionPlugin>

class SimVisModulePlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.compphys.SimVis.SimVisModulePlugin")
public:
    virtual void registerTypes(const char *uri);
};

#endif // SIMVISMODULEPLUGIN_H
