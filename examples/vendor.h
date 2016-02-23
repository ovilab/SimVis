#ifndef VENDOR_H
#define VENDOR_H

#include <QQmlEngine>
#include <QCoreApplication>

#include "core/simvisplugin.h"

namespace qpm {

void init(const QCoreApplication &app, QQmlEngine &engine) {
    // Add qml components
    engine.addImportPath(QStringLiteral("qrc:/"));

    // Add C++ components

    // class:"SimVisPlugin" uri:"SimVis"
    SimVisPlugin pluginInstance_SimVisPlugin;
    pluginInstance_SimVisPlugin.registerTypes("SimVis");
    pluginInstance_SimVisPlugin.initializeEngine(&engine, "SimVis");
}

}


#endif // VENDOR_H
