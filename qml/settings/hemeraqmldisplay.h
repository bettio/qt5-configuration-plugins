#ifndef HEMERA_QML_SETTINGS_HEMERAQMLDISPLAY_H
#define HEMERA_QML_SETTINGS_HEMERAQMLDISPLAY_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

#include "hemeraqmlsandbox.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class Orbit;

/**
 * @class PowerSavingStrategy
 * @ingroup HemeraQmlSettings
 *
 * @brief Container for PowerSavingStrategy enum.
 */
class PowerSavingStrategy : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(PowerSavingStrategy)

public:
    /**
     * @brief Defines the powersaving strategy for a Display.
     *
     * @note This is a Flag, and as such you can specify multiple values.
     *
     * @note In cases such as X11 display, the powersaving strategy might be overwritten by
     *       configuration files (xorg.conf). Make sure you are not managing powersave in both places.
     *
     * @sa Display
     */
    enum Strategy {
        /// Do not attempt to perform powersaving on the display.
        NoPowerSaving = 0,
        /// Stop applications in the orbit handler. Implies TurnOffScreen.
        StopApplications = 1 << 0,
        /// Turn off the screen.
        TurnOffScreen = 1 << 1,
        AllowMaintenance = 1 << 2
    };
    Q_ENUM(Strategy)
    Q_DECLARE_FLAGS(Strategies, Strategy)
    Q_FLAGS(Strategies)
};

/**
 * @class Display
 * @ingroup HemeraQmlSettings
 *
 * @brief Display is the base QML object for defining Displays in a Hemera @ref Hemera::Qml::Settings::GUIAppliance "GUI Appliance".
 *
 * Display enables a physical display or framebuffer, and associates an Orbit Handler to it. Display
 * is not meant to be used directly: instead, you should use one of its subclasses, which defines the display
 * server or painting device to be used.
 *
 * At the moment, the following displays are available:
 *
 *  - @ref Hemera::Qml::Settings::WaylandDisplay "Wayland": Wayland is the most modern mechanism available, and meant to run
 *    on GL capable devices with Wayland support. Please enquire with your representative to find out if your device supports
 *    Wayland.
 *  - @ref Hemera::Qml::Settings::EGLFSDisplay "EGL Full Screen": EGL Full Screen, or EGLFS, tries to paint directly to an
 *    EGL framebuffer. It requires a GL capable device, and it is a good alternative to Wayland on those GL capable devices
 *    where Wayland isn't supported. Please note that running Widgets-based applications on EGLFS, although possibly working,
 *    it is not supported, and you are voiding your warranty when doing so. The advised choice is
 *    either LinuxFB or X11. Please enquire with your Hemera representative to learn more.
 *  - @ref Hemera::Qml::Settings::LinuxFBDisplay "LinuxFB": LinuxFB paints directly to the Linux Framebuffer without any intermediary.
 *    It is the most compatible and lightweight method of painting to a screen, but it comes with a price: it works only with
 *    non-GL applications, and is unsuitable for complex use cases. It is the advised choice on those devices where GL is not available
 *    and only one application at a time is displayed on the screen, or there is no heavy multitasking need.
 *  - @ref Hemera::Qml::Settings::X11Display "X11": X11 is the most popular solution for painting on a screen. It supports both
 *    GL and non-GL devices and applications depending on the driver, but carries a relevant overhead and requires a full-fledged
 *    server to be running. It is usually advised on those devices where compatibility issues suggest the usage of X11, or if
 *    applications require X11 to paint on the screen (Java is an example).
 *  - @ref Hemera::Qml::Settings::DirectFBDisplay "DirectFB": DirectFB is a lightweight alternative to X11. It is usually not advised,
 *    unless target device has explicit support for DirectFB.
 *
 * Usually, your device will come with a recommended Display to be used, which will be the one of its default Developer Mode image.
 * It is strongly suggested to stick with that choice, or to enquire with your representative to find out what works best for your
 * use case.
 *
 * Display is also a Sandbox: this is done for those cases, such as X11 and Wayland, where an additional component (server or compositor)
 * is meant to be run. Usually, there's no need to specify any Sandbox-specific parameters to Display (besides name, which is compulsory),
 * but in some cases it might be desirable to limit or allow the compositor to perform further actions.
 *
 * @note Several concepts in Display are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa Hemera::Qml::Settings::GuiAppliance
 */
class Display : public Sandbox
{
    Q_OBJECT
    Q_DISABLE_COPY(Display)

    /// The active orbit of the Display's associated Orbit Handler
    Q_PROPERTY(QString activeOrbit MEMBER m_activeOrbit NOTIFY activeOrbitChanged)
    /**
     * The powersaving strategies of the appliance. Refer to Hemera::Qml::Settings::PowerSavingStrategy::Strategy for possible values.
     * If unset, it defaults to _PowerSavingStrategy.TurnOffScreen_
     */
    Q_PROPERTY(Hemera::Qml::Settings::PowerSavingStrategy::Strategies powerSavingStrategies MEMBER m_powerSavingStrategies)
    /**
     * The idle time, in seconds, of this Display/Orbit Handler. Defaults to 10 minutes.
     *
     * @note This settings does not affect anything if no powersaving strategy is set.
     */
    Q_PROPERTY(quint32 idleTime MEMBER m_idleTime)

public:
    explicit Display(QObject *parent = 0);
    virtual ~Display();

Q_SIGNALS:
    void idleTimeExpired();
    void activeOrbitChanged();

private:
    QString m_activeOrbit;
    QString m_name;
    Hemera::Qml::Settings::PowerSavingStrategy::Strategies m_powerSavingStrategies;
    quint32 m_idleTime;
};

}

}

}

Q_DECLARE_OPERATORS_FOR_FLAGS(Hemera::Qml::Settings::PowerSavingStrategy::Strategies)

#endif // HEMERA_QML_SETTINGS_HEMERAQMLDISPLAY_H
