#ifndef HEMERA_QML_SETTINGS_HEMERAQMLLINUXFBDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLLINUXFBDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlframebufferdisplay.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class Orbit;

/**
 * @class LinuxFBDisplay
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Display using LinuxFB to paint to the screen.
 *
 * LinuxFBDisplay is a FramebufferDisplay, and as such you need to specify the framebuffer device it runs on.
 *
 * LinuxFB paints directly to the Linux Framebuffer without any intermediary.
 * It is the most compatible and lightweight method of painting to a screen, but it works only with non-GL applications.
 * It is the advised choice on those devices where GL is not available
 * and only one application at a time is displayed on the screen, or there is no heavy multitasking need.
 *
 * @sa Hemera::Qml::Settings::FramebufferDisplay
 */
class LinuxFBDisplay : public FramebufferDisplay
{
    Q_OBJECT
    Q_DISABLE_COPY(LinuxFBDisplay)

public:
    explicit LinuxFBDisplay(QObject *parent = 0);
    virtual ~LinuxFBDisplay();

private:
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLLINUXFBDISPLAY_H
