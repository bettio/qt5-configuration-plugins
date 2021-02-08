#ifndef HEMERA_QML_SETTINGS_HEMERAQMLWAYLANDONXDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLWAYLANDONXDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlx11display.h"

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * @class WaylandOnXDisplay
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Display which runs a X11 display server with a Wayland compositor running on it, and which runs applications as Wayland clients.
 *
 * X11 is the most popular solution for painting on a screen. It supports both
 * GL and non-GL devices and applications depending on the driver, but carries a relevant overhead and requires a full-fledged
 * server to be running.
 *
 * Wayland is the most modern mechanism available, and meant to run
 * on GL capable devices with Wayland support.
 * Please enquire with your representative to find out if your device supports Wayland. If this is not the
 * case, EGLFS or X11 are the most valid alternatives. 
 *
 * @sa Hemera::Qml::Settings::Display
 */
class WaylandOnXDisplay : public X11Display
{
    Q_OBJECT
    Q_DISABLE_COPY(WaylandOnXDisplay)

public:
    explicit WaylandOnXDisplay(QObject *parent = 0);
    virtual ~WaylandOnXDisplay();
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLWAYLANDONXDISPLAY_H
