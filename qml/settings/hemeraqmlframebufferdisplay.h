#ifndef HEMERA_QML_SETTINGS_HEMERAQMLFRAMEBUFFERDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLFRAMEBUFFERDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmldisplay.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class Orbit;

/**
 * @class FramebufferDisplay
 * @ingroup HemeraQmlSettings
 *
 * @brief Convenience intermediate class for those displays based on direct framebuffer access.
 *
 * @sa Hemera::Qml::Settings::Display
 */
class FramebufferDisplay : public Display
{
    Q_OBJECT
    Q_DISABLE_COPY(FramebufferDisplay)

    /**
     * The full path to the framebuffer device. Defaults to /dev/fb0
     */
    Q_PROPERTY(QString device MEMBER m_device)

public:
    explicit FramebufferDisplay(QObject *parent = 0);
    virtual ~FramebufferDisplay();

private:
    QString m_device;
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLFRAMEBUFFERDISPLAY_H
