#ifndef HEMERA_QML_SETTINGS_HEMERAQMLWAYLANDDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLWAYLANDDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlframebufferdisplay.h"

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * @class WaylandDisplay
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Display which runs a Wayland compositor, and which runs applications as Wayland clients.
 *
 * WaylandDisplay is a FramebufferDisplay, and as such you need to specify the framebuffer device it runs on.
 *
 * Wayland is the most modern mechanism available, and meant to run
 * on GL capable devices with Wayland support.
 * Please enquire with your representative to find out if your device supports Wayland. If this is not the
 * case, EGLFS or X11 are the most valid alternatives.
 *
 * @sa Hemera::Qml::Settings::FramebufferDisplay
 * @sa Hemera::Qml::Settings::EGLFSDisplay
 * @sa Hemera::Qml::Settings::X11Display
 */
class WaylandDisplay : public FramebufferDisplay
{
    Q_OBJECT
    Q_DISABLE_COPY(WaylandDisplay)

public:
    explicit WaylandDisplay(QObject *parent = 0);
    virtual ~WaylandDisplay();

private:
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLWAYLANDDISPLAY_H
