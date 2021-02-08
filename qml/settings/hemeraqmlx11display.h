#ifndef HEMERA_QML_SETTINGS_HEMERAQMLX11DISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLX11DISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmldisplay.h"

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * @class X11Display
 * @ingroup HemeraQmlSettings
 *
 * @brief Represents a Display which runs a X11 display server, and which runs applications as X11 clients.
 *
 * X11 is the most popular solution for painting on a screen. It supports both
 * GL and non-GL devices and applications depending on the driver, but carries a relevant overhead and requires a full-fledged
 * server to be running. It is usually advised on those devices where compatibility issues suggest the usage of X11, or if
 * applications require X11 to paint on the screen (Java is an example).
 *
 * You will need to specify which display to run X11 on (usually :0), and whether to use or not a window
 * manager. While this is not strictly needed, for the nature of X11 it is strongly advised to run
 * a Window Manager unless you plan on running one single application at all times.
 *
 * @sa Hemera::Qml::Settings::Display
 */
class X11Display : public Display
{
    Q_OBJECT
    Q_DISABLE_COPY(X11Display)

    /// The display to run X11 on. Defaults to :0.
    Q_PROPERTY(QString display MEMBER m_display)
    /// Whether to use a Window Manager or not. Defaults to true.
    Q_PROPERTY(bool useWindowManager MEMBER m_useWindowManager)

public:
    explicit X11Display(QObject *parent = 0);
    virtual ~X11Display();

Q_SIGNALS:
    void idleTimeExpired();
    void activeOrbitChanged();

private:
    QString m_display;
    bool m_useWindowManager;
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLX11DISPLAY_H
