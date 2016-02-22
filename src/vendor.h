#ifndef QPM_H
#define QPM_H

#include <QQmlApplicationEngine>
#include <QGuiApplication>
#include <QtQml>

#include "core/simvisplugin.h"

namespace qpm {

void init(const QGuiApplication &app, const QQmlApplicationEngine &engine) {
    SimVisPlugin plugin;
    plugin.registerTypes("SimVis");
}

}

#endif // QPM_H

