#ifndef HEMERA_QML_SETTINGS_HEMERAQMLGUIAPPLIANCE_H
#define HEMERA_QML_SETTINGS_HEMERAQMLGUIAPPLIANCE_H

#include "hemeraqmlappliance.h"

#include <QtQml/QQmlListProperty>

namespace Hemera {

namespace Qml {

namespace Settings {

class Display;

/**
 * @class GUIAppliance
 * @ingroup HemeraQmlSettings
 *
 * @brief GUIAppliance is the base object for Gravity configurations on systems which feature an User Interface.
 * Each system is an appliance, and as such a system is represented by one and only one appliance.
 *
 * GUIAppliance is a subclass of Appliance, and the only difference is that GUIAppliance requires a list
 * of @ref Hemera::Qml::Settings::Display "Displays" to manage.
 *
 * An example of a GUI Appliance object is:
 *
 * @code
 * GUIAppliance {
 *      name: "My GUI appliance"
 *      residentOrbit: "comMyResidentOrbit"
 *      updateStrategies: UpdateStrategy.AllowOTA | UpdateStrategy.AllowRemovableMedia | UpdateStrategy.CheckForUpdatesWhenIdle
 *
 *      WaylandDisplay {
 *          name: "My Wayland Display on fb0"
 *          activeOrbit: "comMyOrbitOne"
 *          device: "/dev/fb0"
 *      }
 *
 *      EGLFSDisplay {
 *          name: "My EGLFS Display on fb1"
 *          activeOrbit: "comMyOrbitTwo"
 *          device: "/dev/fb1"
 *      }
 * }
 * @endcode
 *
 * @sa Hemera::Qml::Settings::Appliance
 * @sa Hemera::Qml::Settings::Display
 */
class GUIAppliance : public Appliance
{
    Q_OBJECT
    Q_DISABLE_COPY(GUIAppliance)

    /**
     * A list of @ref Hemera::Qml::Settings::Display "Displays" to manage.
     */
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::Display> displays READ displays)

    Q_CLASSINFO("DefaultProperty", "displays")

public:
    explicit GUIAppliance(QObject *parent = 0);
    virtual ~GUIAppliance();

    QQmlListProperty<Hemera::Qml::Settings::Display> displays();

private:
    QList< Display* > m_displays;
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLGUIAPPLIANCE_H
