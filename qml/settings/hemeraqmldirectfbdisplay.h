#ifndef HEMERA_QML_SETTINGS_HEMERAQMLDIRECTFBDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLDIRECTFBDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlframebufferdisplay.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class Orbit;

/**
 * @class DirectFBDisplay
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Display using DirectFB to paint to the screen.
 *
 * DirectFBDisplay is a FramebufferDisplay, and as such you need to specify the framebuffer device it runs on.
 *
 * DirectFB is a lightweight alternative to X11. It is usually not advised,
 * unless target device has explicit support for DirectFB.
 *
 * @sa Hemera::Qml::Settings::FramebufferDisplay
 */
class DirectFBDisplay : public FramebufferDisplay
{
    Q_OBJECT
    Q_DISABLE_COPY(DirectFBDisplay)

public:
    explicit DirectFBDisplay(QObject *parent = 0);
    virtual ~DirectFBDisplay();

private:
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLDIRECTFBDISPLAY_H
