#ifndef SIMVISPLUGIN_H
#define SIMVISPLUGIN_H

#include <QQmlExtensionPlugin>

class SimVisPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org.compphys.SimVis.SimVisPlugin")

public:
    void registerTypes(const char *uri);
};

#endif // SIMVISPLUGIN_H
