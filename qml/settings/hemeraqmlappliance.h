#ifndef HEMERA_QML_SETTINGS_HEMERAQMLAPPLIANCE_H
#define HEMERA_QML_SETTINGS_HEMERAQMLAPPLIANCE_H

#include <QtCore/QObject>
#include <QtQml/QQmlListProperty>

/**
 * @defgroup HemeraQmlSettings Hemera QML Settings
 *
 * Hemera QML Settings is the QML plugin used for Hemera Configuration. In this module, you will
 * find documentation for every QML object made available by the configuration system (apart from its extensions).
 *
 * Given these classes should not be used from C++, only properties and enums are documented.
 */

namespace Hemera {

namespace Qml {

namespace Settings {

class Orbit;
class UpdateStrategy;

/**
 * @class Appliance
 * @ingroup HemeraQmlSettings
 *
 * @brief Appliance is the base object for Gravity configurations. Each system is an appliance, and as such a system
 * is represented by one and only one appliance.
 *
 * Appliance represents an Headless appliance - you should use its subclass GUIAppliance to support displays.
 *
 * An example of an Appliance object is:
 *
 * @code
 * Appliance {
 *      name: "My appliance"
 *      activeOrbit: "comMyOrbit"
 *      residentOrbit: "comMyResidentOrbit"
 * }
 * @endcode
 *
 * @note Several concepts in Appliance are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa GUIAppliance
 */
class Appliance : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Appliance)

    /**
     * The active orbit of the system. If you wish to use the appliance's Orbit Handler, you should set this to a meaningful value.
     * Usually, when building a GUIAppliance, this value is left unset and active orbits are specified for each Display's Orbit
     * Handler.
     */
    Q_PROPERTY(QString activeOrbit MEMBER m_activeOrbit NOTIFY activeOrbitChanged)
    /// The name of this appliance
    Q_PROPERTY(QString name MEMBER m_name)
    /**
     * The resident orbit of the system. If you wish to have any persistent application or daemon, this Orbit is the first one
     * to be brought up, the last one to be shutdown, and is kept active at all times.
     */
    Q_PROPERTY(QString residentOrbit MEMBER m_residentOrbit)
    /**
     * The update strategies of the appliance. Refer to @ref Hemera::Qml::Settings::UpdateStrategy for possible values.
     * If unset, the device won't be updateable.
     */
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy> updateStrategies READ updateStrategies)
    /**
     * When the system attempts to perform an incremental update, shutdown all running orbits and inject the specified
     * orbit during the update. This is useful if you want to avoid background updates at all times. If left empty,
     * no orbit will be injected.
     */
    Q_PROPERTY(QString updateOrbit MEMBER m_updateOrbit)

public:
    explicit Appliance(QObject *parent = 0);
    virtual ~Appliance();

    QQmlListProperty<Hemera::Qml::Settings::UpdateStrategy> updateStrategies();

Q_SIGNALS:
    void activeOrbitChanged();

private:
    QString m_activeOrbit;
    QString m_name;
    QString m_residentOrbit;
    QString m_updateOrbit;

    QList< UpdateStrategy* > m_updateStrategies;
};

}

}

}

#endif // HEMERA_QML_SETTINGS_HEMERAQMLAPPLIANCE_H
