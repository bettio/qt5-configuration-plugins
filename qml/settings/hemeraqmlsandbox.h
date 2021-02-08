#ifndef HEMERA_QML_SETTINGS_HEMERAQMLSANDBOX_H
#define HEMERA_QML_SETTINGS_HEMERAQMLSANDBOX_H

#include <QtCore/QObject>
#include <QtCore/QFlags>
#include <QtCore/QStringList>

#include <QtQml/QQmlListProperty>

#include "hemeraqmllanguagesupport.h"

namespace Hemera {

namespace Qml {

namespace Settings {

class Device;

/**
 * @class OrbitFeatures
 * @ingroup HemeraQmlSettings
 *
 * @brief Container for Features enum.
 */
class OrbitFeatures : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(OrbitFeatures)

public:
    /**
     * @brief Defines the available Features for a Sandbox.
     *
     * @note This is a Flag, and as such you can specify multiple values.
     *
     * @sa Hemera::Qml::Settings::Sandbox
     */
    enum Feature {
        /// No features allowed
        NoFeatures = 0,
        /**
         * Allows access to the audio cards, and to the local PulseAudio server. Needed to
         * play or record audio.
         */
        Audio = 1 << 0,
        /**
         * Allows access to the video framebuffers and video devices, such as cameras. Usually needed
         * for every Sandbox which needs to paint on the screen.
         */
        Video = 1 << 1,
        /**
         * Allows access to serial ports on the system, if any.
         */
        SerialPorts = 1 << 2,
        /**
         * Allows access to TTYs or consoles, if any.
         */
        Console = 1 << 3,
        /**
         * Allows access to Printers in the system, if any.
         */
        Printers = 1 << 4,
        /**
         * Allows access to disks on the system, such as removable media.
         */
        Disks = 1 << 5,
        /**
         * Allows access to the local Hyperspace network, and allows the application to register Gates
         * on Hyperspace. Refer to Hyperspace documentation to learn more.
         */
        Hyperspace = 1 << 6,
        /**
         * Allows access to network management. Please note this feature does not perform any hard restriction
         * to local or remote networks.
         */
        Network = 1 << 7,

        // Software management
        /**
         * Allow applications in the Sandbox to manipulate applications (install, remove, update).
         */
        ManageApplications = 1 << 11,
        /**
         * Allow applications in the Sandbox to manage software repositories. Please note this is a dangerous and
         * destructive operation, and should be handled with care.
         */
        ManageApplicationSources = 1 << 12,
        /**
         * Allow applications in the Sandbox to explicitly trigger a system update.
         */
        UpdateSystem = 1 << 13,
        /**
         * Allows an application in the Sandbox to register itself as a Planet.
         */
        Planet = 1 << 14,


        /// Other features
        /**
         * Enables the use of the Hemera::ApplianceCrypto APIs, accessing appliance-wide certificates.
         *
         * @note Use this API wisely, as it might pose a security threat to the whole appliance cluster.
         */
        ApplianceCrypto = 1 << 20,
        /**
         * Enables the use of the Hemera::DeviceManagement APIs, accessing system configuration features.
         *
         * @note Use this API wisely, as it might pose a security threat to the device, or compromise the Appliance.
         */
        DeviceManagement = 1 << 21,
        /**
         * Enables the use of the Hemera::DeviceManagement APIs for power management (Shutdown, Reboot, Suspend).
         */
        DevicePowerManagement = 1 << 22,
        /// This enables groups such as cdrom, floppy and tape
        LegacyDevices = 1 << 31
    };
    Q_DECLARE_FLAGS(Features, Feature)
    Q_FLAGS(Features)
    Q_ENUM(Feature)
};

/**
 * @class Sandbox
 * @ingroup HemeraQmlSettings
 *
 * @brief Sandbox is the base object for any entity in Hemera which is meant to be run as a slave to Gravity. It is
 * not meant to be used directly, but through one of its subclasses:
 *
 *  - Hemera::Qml::Settings::Application : Application is a Sandbox which generates Orbital Applications
 *  - Hemera::Qml::Settings::Orbit : Orbit is the most basic implementation of a Sandbox running any number of Applications
 *  - Hemera::Qml::Settings::Display : Display encloses in the Sandbox the compositor or server, if any.
 *
 * Sandbox is mainly a container for application, which allows to downscale their privileges and give them access
 * to a limited and controlled number of resources. The configuration can be fine tuned to give or take from the Sandbox
 * virtually any privilege. The main mean of control are @ref Hemera::Qml::Settings::OrbitFeatures::Features "Features".
 *
 * For every Sandbox, a separated session is created with a dedicated Control Group. Gravity watches over every Sandbox
 * to ensure everything is running smoothly and limits via its internal policies any unallowed actions requested by
 * the applications.
 *
 * @par Relationship with Orbits
 *
 * In Hemera, usually Sandboxes and Orbits are treated as synonyms. While this is almost correct, and in fact hidden
 * by the fact that the Sandbox term is never used nor referred in public APIs, an Orbit is an implementation of
 * a Sandbox which can be launched by Gravity on demand. Orbital Applications are Orbits and Sandboxes.
 *
 * @note Several concepts in Sandbox are better explained in @ref BaseHemeraConcepts "Base Hemera Concepts"
 *
 * @sa Hemera::Qml::Settings::Orbit
 * @sa Hemera::Qml::Settings::Application
 */
class Sandbox : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(Sandbox)

    /// The name of this Sandbox. Always compulsory.
    Q_PROPERTY(QString name MEMBER m_name)
    /**
     * Whether the Sandbox/Orbit should be kept alive when deemed inactive.
     * If the Sandbox is an Orbit or an Orbital Application, Gravity will strive to stop applications and
     * keep them alive instead of shutting down the orbit entirely.
     *
     * While this allows for a faster orbit switch, it is also heavier on memory and CPU.
     */
    Q_PROPERTY(bool keepAlive MEMBER m_keepAlive)
    /**
     * The enabled features of this Sandbox. Refer to @ref Hemera::Qml::Settings::OrbitFeatures::Features "Features" for possible values.
     * If unset, it defaults to _Features.NoFeatures_
     */
    Q_PROPERTY(Hemera::Qml::Settings::OrbitFeatures::Features features MEMBER m_features)
    /**
     * Additional UNIX environment which should be passed to applications and everything living in the Sandbox.
     */
    Q_PROPERTY(QString environment MEMBER m_environment)
    /**
     * When features are not enough, it is possible to specify a list of devices to be accessed or denied from
     * within the Sandbox.
     *
     * @sa Hemera::Qml::Settings::Device
     */
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::Device> devices READ devices)
    Q_PROPERTY(QQmlListProperty<Hemera::Qml::Settings::LanguageSupport> languageSupport READ languageSupport)
    /**
     * Whether to allow updates or not while this Sandbox is active. Defaults to false.
     */
    Q_PROPERTY(bool allowUpdates MEMBER m_allowUpdates)
    /**
     * Whether to run this Sandbox as root. Defaults to false.
     *
     * @note Please enquire with your Hemera representative before enabling this option. Running Sandboxes as root is
     *       *NOT* advised and just partially supported. Giving the Sandbox additional privileges should be done via
     *       features. Since this partially voids your warranty, you need to enable another undocumented option which you
     *       can ask your Hemera representative about.
     */
    Q_PROPERTY(bool runAsRoot MEMBER m_runAsRoot)
    Q_PROPERTY(bool IUnderstandItIsASecurityRisk MEMBER m_IUnderstandItIsASecurityRisk)

    Q_CLASSINFO("DefaultProperty", "languageSupport")

public:
    explicit Sandbox(QObject *parent = 0);
    virtual ~Sandbox();

    QQmlListProperty<Hemera::Qml::Settings::Device> devices();
    QQmlListProperty<Hemera::Qml::Settings::LanguageSupport> languageSupport();

private:
    bool m_keepAlive;
    OrbitFeatures::Features m_features;
    QString m_environment;
    QString m_name;
    QList< Device* > m_devices;
    QList< LanguageSupport* > m_languageSupport;
    bool m_allowUpdates;
    bool m_runAsRoot;
    bool m_IUnderstandItIsASecurityRisk;

    friend class Gravity;
};

}

}

}

Q_DECLARE_OPERATORS_FOR_FLAGS(Hemera::Qml::Settings::OrbitFeatures::Features)

#endif // HEMERA_QML_SETTINGS_HEMERAQMLSANDBOX_H
