#ifndef HEMERA_QML_SETTINGS_HEMERAQMLEGLFSDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLEGLFSDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlframebufferdisplay.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class Orbit;

/**
 * @class EGLFSDisplay
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Display which paints directly to the EGL framebuffer.
 *
 * EGLFSDisplay is a FramebufferDisplay, and as such you need to specify the framebuffer device it runs on.
 *
 * EGL Full Screen, or EGLFS, tries to paint directly to an
 * EGL framebuffer. It requires a GL capable device, and it is a good alternative to Wayland on those GL capable devices
 * where Wayland isn't supported.
 *
 * Please note that running Widgets-based applications on EGLFS, although possibly working,
 * it is not supported, and you are voiding your warranty when doing so. The advised choice is
 * either LinuxFB or X11. Please enquire with your Hemera representative to learn more.
 *
 * @sa Hemera::Qml::Settings::FramebufferDisplay
 * @sa Hemera::Qml::Settings::WaylandDisplay
 */
class EGLFSDisplay : public FramebufferDisplay
{
    Q_OBJECT
    Q_DISABLE_COPY(EGLFSDisplay)

public:
    explicit EGLFSDisplay(QObject *parent = 0);
    virtual ~EGLFSDisplay();

private:
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLEGLFSDISPLAY_H
