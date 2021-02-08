/*
 *
 */

#include "gravitycompileoperation.h"

#include <HemeraCore/Literals>
#include <HemeraCore/ServiceManager>

#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QTimer>
#include <QtCore/QSettings>
#include <QtCore/QProcess>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonDocument>

#include <QtQml/QQmlComponent>
#include <QtQml/QQmlEngine>

#include <iostream>

#include "gravitycompilerplugin.h"

#include <pluginsconfig.h>

#include <qml/settings/hemeraqmlappliance.h>
#include <qml/settings/hemeraqmldirectfbdisplay.h>
#include <qml/settings/hemeraqmleglfsdisplay.h>
#include <qml/settings/hemeraqmlguiappliance.h>
#include <qml/settings/hemeraqmllinuxfbdisplay.h>
#include <qml/settings/hemeraqmlupdatestrategy.h>
#include <qml/settings/hemeraqmlwaylanddisplay.h>
#include <qml/settings/hemeraqmlwaylandonxdisplay.h>
#include <qml/settings/hemeraqmlx11display.h>

// PATHS
#define SYSTEMD_SYSTEM_PATH QStringLiteral("/lib/systemd/system/")
#define SYSTEMD_USER_PATH QStringLiteral("/usr/lib/systemd/user/")

static QStringList reservedNames = { QStringLiteral("control"), QStringLiteral("manager"), QStringLiteral("hemera"), QStringLiteral("ResidentOrbit") };
static QRegExp nameValidator = QRegExp(QStringLiteral("[a-zA-Z0-9]*"), Qt::CaseInsensitive);

GravityCompileOperation::GravityCompileOperation(const Hemera::Generators::CompilerConfiguration& configuration,
                                                 QObject* object, QQmlEngine* engine, Type type, GravityCompilerPlugin* parent)
    : Operation(object)
    , m_plugin(parent)
    , m_settings(configuration)
    , m_object(object)
    , m_type(type)
    , m_qmlEngine(engine)
{
}


GravityCompileOperation::~GravityCompileOperation()
{
}

void GravityCompileOperation::startImpl()
{
    // Get the name
    if (m_object->property("applicationId").isValid()) {
        m_name = m_object->property("applicationId").toString();
        m_name.remove(QLatin1Char('.'));
    } else {
        m_name = m_object->property("name").toString();
        // It's not an orbital application. Verify we're not attempting to compile debug support...
        if (m_settings.debugSupport()) {
            setFinishedWithError(Hemera::Literals::literal(Hemera::Literals::Errors::badRequest()),
                                 QStringLiteral("Requested to compile debug support, but target is not an Orbital Application."));
            return;
        }
    }

    // Cleanup?
    if (m_settings.cleanup()) {
        cleanup();
    }

    // Load the component
    switch (m_type) {
        case Type::Appliance:
            QTimer::singleShot(0, this, SLOT(compileAppliance()));
            break;
        case Type::Sandbox:
            QTimer::singleShot(0, this, SLOT(compileSandbox()));
            break;
        default:
            setFinishedWithError(QStringLiteral("QML Object corrupted!"), QString());
            break;
    }
}

void GravityCompileOperation::finalizeUsers()
{
    QDir dir;
    dir.mkpath(QString::fromLatin1("%1%2/").arg(m_settings.prefix(), StaticConfig::gravityUsersListPath()));

    // Now, generate the user list for the package script. This way, the package will take care
    // of generating the correct users for us.
    QString filename = m_name;
    if (filename.isEmpty()) {
        filename = QStringLiteral("Appliance");
    }

    QString usersListPath = QString::fromLatin1("%1%2/%3.userlist").arg(m_settings.prefix(), StaticConfig::gravityUsersListPath(), filename);

    std::cout << "\tGenerating user list file..";

    QFile usersListDestination(usersListPath);
    if (!usersListDestination.open(QIODevice::WriteOnly)) {
        setFinishedWithError(QString::fromLatin1("Could not open %1 for writing!").arg(usersListPath), QString());
        return;
    }
    std::cout << ".";

    QString userPayload;
    for (QHash<QString, QStringList>::const_iterator i = m_userList.constBegin(); i != m_userList.constEnd(); ++i) {
        userPayload.append(i.key());
        userPayload.append(QLatin1Char(':'));
        userPayload.append(i.value().join(QLatin1Char(',')));
        userPayload.append(QLatin1Char('\n'));
    }

    usersListDestination.write(userPayload.toLatin1());
    usersListDestination.flush();
    usersListDestination.close();
    std::cout << " Success!" << std::endl;

    if (m_settings.invokeUserManager()) {
        std::cout << "Invoking user manager..." << std::endl;

        // Recompile the orbit
        QProcess *process = new QProcess(this);

        // Handle the function pointer overload...
        void (QProcess::*finishedSignal)(int exitCode) = &QProcess::finished;

        connect(process, finishedSignal, [this, finishedSignal] (int exitCode) {
                if (exitCode != 0) {
                    std::cout << "User manager failed!" << std::endl;
                    return;
                }

                std::cout << "Users updated successfully." << std::endl;
        });

        QStringList userlists;
        for (const QString &name : m_plugin->m_takenNames) {
            userlists << QString::fromLatin1("%1%2%3%4.userlist").arg(m_settings.prefix(), StaticConfig::gravityUsersListPath(),
                                                                      QDir::separator(), name);
        }

        process->start(QString::fromLatin1("%1%2%3").arg(StaticConfig::hemeraToolsInstallDir(),
                                                         QDir::separator(), QStringLiteral("gravity-user-manager")),
                       QStringList() << QStringLiteral("--create") << userlists);

        process->waitForFinished();
    }

    setFinished();
}

QByteArray GravityCompileOperation::payload(const QString &file)
{
    QDir dataDir;

    if (m_settings.inTreeCompile()) {
        dataDir = QDir(m_settings.prefix() + StaticConfig::hemeraDataDir());
    } else {
        dataDir = QDir(StaticConfig::hemeraDataDir());
    }

    return m_plugin->baseCompiler()->payload(dataDir.absoluteFilePath(file));
}

QStringList GravityCompileOperation::featuresToGroups(Hemera::Qml::Settings::OrbitFeatures::Features features)
{
    // Rely on the private headers to avoid magic numbers
    using namespace Hemera::Qml::Settings;

    // List features for groups
    QStringList groups;

    if (features & OrbitFeatures::Audio) {
        groups.append(QStringLiteral("audio"));
    }
    if (features & OrbitFeatures::Console) {
        groups.append(QStringLiteral("tty"));
    }
    if (features & OrbitFeatures::Disks) {
        groups.append(QStringLiteral("disk"));
    }
    if (features & OrbitFeatures::LegacyDevices) {
        groups.append(QStringLiteral("floppy"));
        groups.append(QStringLiteral("tape"));
        groups.append(QStringLiteral("cdrom"));
    }
    if (features & OrbitFeatures::Printers) {
        groups.append(QStringLiteral("lp"));
    }
    if (features & OrbitFeatures::SerialPorts) {
        groups.append(QStringLiteral("dialout"));
    }
    if (features & OrbitFeatures::Video) {
        groups.append(QStringLiteral("video"));
    }
    if (features & OrbitFeatures::Hyperspace) {
        groups.append(QStringLiteral("hyperspace"));
    }
    if (features & OrbitFeatures::Network) {
        groups.append(QStringLiteral("radio"));
    }

    // Software management
    if (features & OrbitFeatures::ManageApplications) {
        groups.append(QStringLiteral("hemera-appmgmt"));
    }
    if (features & OrbitFeatures::ManageApplicationSources) {
        groups.append(QStringLiteral("hemera-appsrcs"));
    }
    if (features & OrbitFeatures::UpdateSystem) {
        groups.append(QStringLiteral("hemera-insysup"));
    }
    if (features & OrbitFeatures::ApplianceCrypto) {
        groups.append(QStringLiteral("hemera-acrypto"));
    }
    if (features & OrbitFeatures::DeviceManagement) {
        groups.append(QStringLiteral("hemera-dmgmt"));
    }
    if (features & OrbitFeatures::DevicePowerManagement) {
        groups.append(QStringLiteral("hemera-pwrmgmt"));
    }

    return groups;
}

void GravityCompileOperation::cleanup()
{
    std::cout << "Cleaning up previous compilations..." << std::endl;
    // Let's get our target dir first
    QDir userDir(m_settings.prefix() + SYSTEMD_USER_PATH);
    QDir systemDir(m_settings.prefix() + SYSTEMD_SYSTEM_PATH);

    userDir.setFilter(QDir::Files | QDir::NoSymLinks);
    systemDir.setFilter(QDir::Files | QDir::NoSymLinks);

    // Cache all display names
    QStringList starNames;
    if (m_type == Type::Appliance) {
        QQmlListReference displays(m_object, "displays", m_qmlEngine);

        for (int i = 0; i < displays.count(); ++i) {
            starNames.append(displays.at(i)->property("name").toString());
        }
    }

    // Headless?
    if (!m_object->property("activeOrbit").toString().isEmpty() || !m_object->property("residentOrbit").toString().isEmpty()) {
        starNames.append(QStringLiteral("Headless"));
    }

    // We have to cleanup only what's inside our chosen files
    for (const QFileInfo &file : userDir.entryInfoList(QStringList() << QStringLiteral("hemera-orbit-*") << QStringLiteral("hemera-star-*"))) {
        QString orbitName = file.baseName().split(QStringLiteral("hemera-orbit-")).last().split(QStringLiteral("hemera-star-")).last();

        if (m_name != orbitName && !starNames.contains(orbitName)) {
            continue;
        }

        std::cout << "\tCleaning up user targets for " << orbitName.toLatin1().constData() << " .";
        QFile::remove(file.absoluteFilePath());
        // Remove wants & co
        QDir wants(file.absoluteFilePath() + QStringLiteral(".wants"));
        wants.removeRecursively();
        std::cout << ".";

        QDir requires(file.absoluteFilePath() + QStringLiteral(".requires"));
        requires.removeRecursively();
        std::cout << ". Done." << std::endl;
    }

    for (const QFileInfo &file : systemDir.entryInfoList(QStringList() << QStringLiteral("hemera-orbit-*") << QStringLiteral("hemera-star-*"))) {
        QString orbitName = file.baseName().split(QStringLiteral("hemera-orbit-")).last().split(QStringLiteral("hemera-star-")).last();
        orbitName.remove(QLatin1Char('@'));

        if (m_name != orbitName && !starNames.contains(orbitName)) {
            continue;
        }

        std::cout << "\tCleaning up system service for " << orbitName.toLatin1().constData() << " ...";
        QFile::remove(file.absoluteFilePath());

        if (file.baseName().startsWith(QStringLiteral("hemera-star-"))) {
            // Remove also the symlink into graphical target.
            QFile::remove(QString::fromLatin1("%1%2graphical.target.requires/hemera-star-%3.service").arg(m_settings.prefix(), SYSTEMD_SYSTEM_PATH, orbitName));
        }

        std::cout << ". Done." << std::endl;
    }

    for (const QString &star : starNames) {
        QDir environment(QString::fromLatin1("%1%2/stars/%3/").arg(m_settings.prefix(), StaticConfig::hemeraEnvironmentPath(), star));

        std::cout << "\tCleaning up environment files for " << star.toLatin1().constData() << " ...";
        environment.removeRecursively();
        std::cout << " Done." << std::endl;
    }

    std::cout << "All the orbits have been cleaned up." << std::endl << std::endl;
}

void GravityCompileOperation::compileAppliance()
{
    // It is time to find out what kind of appliance this is.
    const QMetaObject *baseMO = m_object->metaObject();

    if (qstrcmp(baseMO->className(), Hemera::Qml::Settings::GUIAppliance::staticMetaObject.className()) == 0) {
        compileGUIAppliance();
    } else if (qstrcmp(baseMO->className(), Hemera::Qml::Settings::Appliance::staticMetaObject.className()) == 0) {
        compileHeadlessAppliance();
    } else {
        setFinishedWithError(QStringLiteral("Wrong appliance type"), QString::fromLatin1("The appliance file is representing %1, but this type is not supported "
                                                                                         "by this version of Gravity Compiler.").arg(QLatin1String(baseMO->className())));
        return;
    }
}

void GravityCompileOperation::writeBaseApplianceConf(bool hasGui)
{
    std::cout << "\t\tCompiling base appliance...";

    // Is the name already taken?
    QString filename = m_name;
    if (filename.isEmpty()) {
        filename = QStringLiteral("Appliance");
    }
    if (m_plugin->m_takenNames.contains(filename)) {
        setFinishedWithError(QString::fromLatin1("There are two orbits/appliances named %1! Appliance and Orbit names must be unique.").arg(filename), QString());
        return;
    }

    m_plugin->m_takenNames.append(filename);

    // Let's load the appliance file.
    QSettings appliance(QString::fromLatin1("%1%2/galaxy.conf").arg(m_settings.prefix(), StaticConfig::configGravityPath()),
                        QSettings::IniFormat);
    appliance.clear();

    appliance.beginGroup(QStringLiteral("Galaxy")); {
        std::cout << ".";
        appliance.setValue(QStringLiteral("HasGui"), hasGui);
        appliance.setValue(QStringLiteral("Name"), m_object->property("name").toString());

        // Let's create the main configs for active and resident orbits for the base appliance.
        appliance.beginGroup(QStringLiteral("BaseOrbit")); {
            std::cout << ".";
            if (!m_object->property("activeOrbit").toString().isEmpty()) {
                appliance.setValue(QStringLiteral("ActiveOrbit"), m_object->property("activeOrbit").toString());
            }
            if (!m_object->property("residentOrbit").toString().isEmpty()) {
                appliance.setValue(QStringLiteral("ResidentOrbit"), m_object->property("residentOrbit").toString());
            }
        } appliance.endGroup();
    } appliance.endGroup();

    // Updates?
    QQmlListReference updateStrategies(m_object, "updateStrategies", m_qmlEngine);

    if (updateStrategies.count() > 0) {
        QSettings updateConf(QStringLiteral("%1%2/update.conf").arg(m_settings.prefix(), StaticConfig::configGravityPath()),
                             QSettings::IniFormat);
        if (!m_object->property("updateOrbit").isValid()) {
            updateConf.setValue(QStringLiteral("updateOrbit"), m_object->property("updateOrbit").toString());
        }
        for (int i = 0; i < updateStrategies.count(); ++i) {
            QObject *updateStrategy = updateStrategies.at(i);
            if (qstrcmp(updateStrategy->metaObject()->className(), Hemera::Qml::Settings::AstarteUpdateStrategy::staticMetaObject.className()) == 0) {
                updateConf.beginGroup(QStringLiteral("Astarte")); {
                    updateConf.setValue(QStringLiteral("enabled"), true);
                } updateConf.endGroup();
            } else if (qstrcmp(updateStrategy->metaObject()->className(),
                               Hemera::Qml::Settings::ImageStoreUpdateStrategy::staticMetaObject.className()) == 0) {
                updateConf.beginGroup(QStringLiteral("ImageStore")); {
                    updateConf.setValue(QStringLiteral("enabled"), true);
                    updateConf.setValue(QStringLiteral("endpoint"), updateStrategy->property("endpoint").toString());
                    updateConf.setValue(QStringLiteral("apiKey"), updateStrategy->property("apiKey").toString());
                } updateConf.endGroup();
            } else if (qstrcmp(updateStrategy->metaObject()->className(),
                               Hemera::Qml::Settings::MassStorageUpdateStrategy::staticMetaObject.className()) == 0) {
                updateConf.beginGroup(QStringLiteral("MassStorage")); {
                    updateConf.setValue(QStringLiteral("enabled"), true);
                    updateConf.setValue(QStringLiteral("automount"), updateStrategy->property("automount").toBool());
                } updateConf.endGroup();
            }
        }
    }

    std::cout << ". Success!" << std::endl;
}

void GravityCompileOperation::compileHeadlessAppliance()
{
    std::cout << "\tCompiling as a headless appliance." << std::endl;

    // Let's get our target dir first
    QString userPath = m_settings.prefix() + QStringLiteral("/usr/lib/systemd/user/");
    QString systemPath = m_settings.prefix() + QStringLiteral("/lib/systemd/system/");
    QDir dir;

    dir.mkpath(QString::fromLatin1("%1%2/").arg(m_settings.prefix(), StaticConfig::configGravityPath()));

    // Create the graphical.target.requires dir.
    QString basicRequires = QString::fromLatin1("%1basic.target.requires/").arg(systemPath);
    dir.mkpath(basicRequires);

    writeBaseApplianceConf(false);

    // Headless star?
    if (!m_object->property("activeOrbit").toString().isEmpty() || !m_object->property("residentOrbit").toString().isEmpty()) {
        if (!compileStar(m_object, QStringLiteral("Headless"), 0)) {
            return;
        }

        // Make the service required by the basic target.
        QString service = QString::fromLatin1("hemera-star-%1.service").arg(QStringLiteral("Headless"));
        if (!QFile::link(QString::fromLatin1("../%1").arg(service), QString::fromLatin1("%1%2").arg(basicRequires, service)) && !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1%2!").arg(basicRequires, service), QString());
            return;
        }
    } else {
        setFinishedWithError(QStringLiteral("Attempting to configure an Appliance with no stars or orbits!"), QString());
        return;
    }

    // Stars will have created users
    finalizeUsers();
}

void GravityCompileOperation::compileGUIAppliance()
{
    std::cout << "\tCompiling as a GUI appliance." << std::endl;

    // Let's get our target dir first
    QString userPath = m_settings.prefix() + QStringLiteral("/usr/lib/systemd/user/");
    QString systemPath = m_settings.prefix() + QStringLiteral("/lib/systemd/system/");
    QDir dir;

    dir.mkpath(QString::fromLatin1("%1%2/").arg(m_settings.prefix(), StaticConfig::configGravityPath()));
    dir.mkpath(QString::fromLatin1("%1/etc/dbus-1/system.d/").arg(m_settings.prefix()));
    dir.mkpath(QString::fromLatin1("%1/etc/dbus-1/starbus.d/").arg(m_settings.prefix()));

    writeBaseApplianceConf(true);

    // Proceed in configuring and compiling star support.
    QSettings galaxy(QString::fromLatin1("%1%2/galaxy.conf").arg(m_settings.prefix(), StaticConfig::configGravityPath()),
                        QSettings::NativeFormat);
    galaxy.beginGroup(QStringLiteral("Stars"));

    QQmlListReference stars(m_object, "displays", m_qmlEngine);

    if (stars.count() <= 0) {
        setFinishedWithError(QStringLiteral("Attempting to configure a GUI Appliance with no displays!"), QString());
        return;
    }

    QStringList starNames;

    // Create the graphical.target.requires dir.
    QString graphicalRequires = QString::fromLatin1("%1graphical.target.requires/").arg(systemPath);
    QString basicRequires = QString::fromLatin1("%1basic.target.requires/").arg(systemPath);
    dir.mkpath(graphicalRequires);
    dir.mkpath(basicRequires);

    int tty = 1;
    for (int i = 0; i < stars.count(); ++i) {
        QObject *star = stars.at(i);
        QString starName = star->property("name").toString();

        if (starNames.contains(starName)) {
            setFinishedWithError(QString::fromLatin1("Tried to create a display with the same name, %1, twice.").arg(starName), QString());
            return;
        }

        starNames << starName;

        if (!compileStar(star, starName, tty)) {
            return;
        }

        // Make the service required by the graphical target.
        QString service = QString::fromLatin1("hemera-star-%1.service").arg(starName);
        if (!QFile::link(QString::fromLatin1("../%1").arg(service), QString::fromLatin1("%1%2").arg(graphicalRequires, service)) && !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1%2!").arg(graphicalRequires, service), QString());
            return;
        }

        // Now, for the settings file
        galaxy.beginGroup(starName); {
            if (!star->property("activeOrbit").toString().isEmpty()) {
                galaxy.setValue(QStringLiteral("ActiveOrbit"), star->property("activeOrbit").toString());
            } else {
                setFinishedWithError(QStringLiteral("Attempting to configure a display with no active Orbit!"), QString());
                return;
            }

            if (!star->property("residentOrbit").toString().isEmpty()) {
                galaxy.setValue(QStringLiteral("ResidentOrbit"), star->property("residentOrbit").toString());
            }
        } galaxy.endGroup();

        ++tty;
    }

    // Headless star?
    if (!m_object->property("activeOrbit").toString().isEmpty() || !m_object->property("residentOrbit").toString().isEmpty()) {
        if (!compileStar(m_object, QStringLiteral("Headless"), stars.count())) {
            return;
        }

        // Make the service required by the basic target.
        QString service = QString::fromLatin1("hemera-star-%1.service").arg(QStringLiteral("Headless"));
        if (!QFile::link(QString::fromLatin1("../%1").arg(service), QString::fromLatin1("%1%2").arg(basicRequires, service)) && !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1%2!").arg(basicRequires, service), QString());
            return;
        }
    }

    // Complete the appliance file.
    galaxy.endGroup();
    galaxy.beginGroup(QStringLiteral("Galaxy")); {
        galaxy.setValue(QStringLiteral("Stars"), starNames);
    } galaxy.endGroup();

    // Finalize users, as we created some for the stars.
    finalizeUsers();
}

bool GravityCompileOperation::compileStar(QObject *star, const QString &starName, int tty)
{
    std::cout << "\t\tCompiling Star (display) " << starName.toLatin1().constData() << "...";

    QString userPath = m_settings.prefix() + QStringLiteral("/usr/lib/systemd/user/");
    QString systemPath = m_settings.prefix() + QStringLiteral("/lib/systemd/system/");
    QDir dir;

    // Introspect the display
    const QMetaObject *displayMO = star->metaObject();

    // Let's set up targets for our star.
    // Get the payload
    QString serviceContent = QString::fromLatin1(payload(QStringLiteral("hemera-star-template.service.in")));
    QString targetContent = QString::fromLatin1(payload(QStringLiteral("hemera-star-template.target.in")));
    QString busPolicyContent = QString::fromLatin1(payload(QStringLiteral("allowance-star-template.conf.in")));
    QString starBusPolicyContent = QString::fromLatin1(payload(QStringLiteral("star-bus-policy-star-template.conf.in")));

    // Do the replace dance
    QString environment;
    if (!star->property("environment").toString().isEmpty()) {
        environment = QStringLiteral("Environment=") + star->property("environment").toString();
    }

    serviceContent.replace(QStringLiteral("@NAME@"), starName);
    serviceContent.replace(QStringLiteral("@ENVIRONMENT@"), environment);
    serviceContent.replace(QStringLiteral("@HEMERA_GRAVITY_ENVIRONMENT_DIR@"), StaticConfig::hemeraEnvironmentPath());

    targetContent.replace(QStringLiteral("@NAME@"), starName);

    busPolicyContent.replace(QStringLiteral("@NAME@"), starName);

    starBusPolicyContent.replace(QStringLiteral("@NAME@"), starName);

    // Let's start by creating our files
    QString service = QString::fromLatin1("hemera-star-%1.service").arg(starName);
    QFile destinationFile(systemPath + service);
    if (!destinationFile.open(QIODevice::WriteOnly)) {
        setFinishedWithError(QStringLiteral("Could not open ") + userPath + service + QStringLiteral(" for writing!"), QString());
        return false;
    }

    QString target = QString::fromLatin1("hemera-star-%1.target").arg(starName);
    QFile targetDestination(userPath + target);
    if (!targetDestination.open(QIODevice::WriteOnly)) {
        setFinishedWithError(QStringLiteral("Could not open ") + userPath + target + QStringLiteral(" for writing!"), QString());
        return false;
    }

    QString busPolicy = QString::fromLatin1("%1/etc/dbus-1/system.d/allow-star-%2.conf").arg(m_settings.prefix(), starName);
    QFile busPolicyDestination(busPolicy);
    if (!busPolicyDestination.open(QIODevice::WriteOnly)) {
        setFinishedWithError(QStringLiteral("Could not open ") + busPolicy + QStringLiteral(" for writing!"), QString());
        return false;
    }

    QString starBusPolicy = QString::fromLatin1("%1/etc/dbus-1/starbus.d/policy-star-%2.conf").arg(m_settings.prefix(), starName);
    QFile starBusPolicyDestination(starBusPolicy);
    if (!starBusPolicyDestination.open(QIODevice::WriteOnly)) {
        setFinishedWithError(QStringLiteral("Could not open ") + starBusPolicy + QStringLiteral(" for writing!"), QString());
        return false;
    }

    // Now, let's put in the requires and wants
    QString targetRequires = userPath + target + QStringLiteral(".requires/");
    QString targetWants = userPath + target + QStringLiteral(".wants/");

    dir.mkpath(targetWants);
    dir.mkpath(targetRequires);

    // Parsec
    if (!QFile::link(QStringLiteral("../parsec.service"), QString::fromLatin1("%1parsec.service").arg(targetRequires)) && !m_settings.allowMultilink()) {
        setFinishedWithError(QStringLiteral("Could not create symlink %1parsec.service!").arg(targetRequires), QString());
        return false;
    }

    // Star DBus
    if (!QFile::link(QStringLiteral("../dbus-hemera-star@.socket"),
                                    QStringLiteral("%1dbus-hemera-star@%2.socket").arg(targetRequires, starName)) && !m_settings.allowMultilink()) {
        setFinishedWithError(QStringLiteral("Could not create symlink %1dbus-hemera-star@%2.socket!").arg(targetRequires, starName), QString());
        return false;
    }
    if (!QFile::link(QStringLiteral("../dbus-hemera-star@.service"),
                                    QStringLiteral("%1dbus-hemera-star@%2.service").arg(targetRequires, starName)) && !m_settings.allowMultilink()) {
        setFinishedWithError(QStringLiteral("Could not create symlink %1dbus-hemera-star@%2.service!").arg(targetRequires, starName), QString());
        return false;
    }

    // Create the environment dir
    QString envDir = QString::fromLatin1("%1%2/stars/%3/").arg(m_settings.prefix(), StaticConfig::hemeraEnvironmentPath(), starName);
    dir.mkpath(envDir);
    std::cout << ".";

    QFile envFile(QString::fromLatin1("%1/01-star-configuration.conf").arg(envDir));
    if (!envFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        setFinishedWithError(QString::fromLatin1("Could not open %1/01-star-configuration.conf for writing!").arg(envDir), QString());
        return false;
    }
    QTextStream outEnvFile(&envFile);

    QString environmentPlatform;

    if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::DirectFBDisplay::staticMetaObject.className()) == 0) {
        outEnvFile << "# These variables determine the Framebuffer Device\n";
        outEnvFile << QString::fromLatin1("FB_DEVICE=%1").arg(star->property("device").toString()) << '\n';

        environmentPlatform = QStringLiteral("directfb");

        // No symlinks to do here, we simply need the framebuffer to be up.
    } else if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::EGLFSDisplay::staticMetaObject.className()) == 0) {
        outEnvFile << "# These variables determine the Framebuffer Device\n";
        outEnvFile << QString::fromLatin1("FB_DEVICE=%1").arg(star->property("device").toString()) << '\n';

        environmentPlatform = QStringLiteral("eglfs");

        // No symlinks to do here, we simply need the framebuffer to be up.
    } else if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::LinuxFBDisplay::staticMetaObject.className()) == 0) {
        outEnvFile << "# These variables determine the Framebuffer Device\n";
        outEnvFile << QString::fromLatin1("FB_DEVICE=%1").arg(star->property("device").toString()) << '\n';

        environmentPlatform = QStringLiteral("linuxfb");

        // No symlinks to do here, we simply need the framebuffer to be up.
    } else if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::WaylandDisplay::staticMetaObject.className()) == 0) {
        outEnvFile << "# These variables determine the Framebuffer Device\n";
        outEnvFile << QString::fromLatin1("FB_DEVICE=%1").arg(star->property("device").toString()) << '\n';
        outEnvFile << QString::fromLatin1("TTY=%1").arg(tty) << '\n';
        outEnvFile << "# These variables determine the socket options for Weston and Wayland clients\n";
        outEnvFile << QStringLiteral("WAYLAND_DISPLAY=%1/%2/systemcompositor/wayland-0").arg(StaticConfig::hemeraStarsRuntimeDir(), starName) << '\n';
        outEnvFile << QStringLiteral("WAYLAND_SERVER_SOCKET_DIR=%1/%2/systemcompositor").arg(StaticConfig::hemeraStarsRuntimeDir(), starName) << '\n';
        outEnvFile << "# These variables determine the startup options for Weston, non adaptation-specific\n";
        outEnvFile << QString::fromLatin1("WESTON_GENERIC_ARGUMENTS=\"--device=%1 --tty=%2\"").arg(star->property("device").toString()).arg(tty) << '\n';

        // Generate weston systemd service
        QString westonContent = QString::fromLatin1(payload(QStringLiteral("weston-star-template.service.in")));
        westonContent.replace(QStringLiteral("@NAME@"), starName);
        westonContent.replace(QStringLiteral("@TTY_NUMBER@"), QString::number(tty));

        // Let's start by creating our files
        QString westonService = QStringLiteral("weston-star-%1.service").arg(starName);
        QFile westonServiceFile(userPath + westonService);
        if (!westonServiceFile.open(QIODevice::WriteOnly)) {
            setFinishedWithError(QStringLiteral("Could not open ") + userPath + westonService + QStringLiteral(" for writing!"), QString());
            return false;
        }
        westonServiceFile.write(westonContent.toUtf8());
        westonServiceFile.flush();
        westonServiceFile.close();
        std::cout << ".";

        // Create symlink
        if (!QFile::link(QStringLiteral("../weston-star-%1.service").arg(starName), QStringLiteral("%1weston-star-%2.service").arg(targetRequires, starName)) &&
            !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1weston-star-%2.service!").arg(targetRequires, starName), QString());
            return false;
        }

        environmentPlatform = QStringLiteral("wayland");

    } else if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::WaylandOnXDisplay::staticMetaObject.className()) == 0) {
        outEnvFile << "# These variables determine the Display for the X11 display server\n";
        outEnvFile << QString::fromLatin1("DISPLAY=%1").arg(star->property("display").toString()) << '\n';
        outEnvFile << "# These variables determine the socket options for Weston and Wayland clients\n";
        outEnvFile << QStringLiteral("WAYLAND_DISPLAY=%1/%2/systemcompositor/wayland-0").arg(StaticConfig::hemeraStarsRuntimeDir(), starName) << '\n';
        outEnvFile << QStringLiteral("WAYLAND_SERVER_SOCKET_DIR=%1/%2/systemcompositor").arg(StaticConfig::hemeraStarsRuntimeDir(), starName) << '\n';

        std::cout << ".";

        // We require xorg target to be up
        if (!QFile::link(QString::fromLatin1("../xorg.target"), QString::fromLatin1("%1xorg.target").arg(targetRequires)) && !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1xorg.target!").arg(targetRequires), QString());
            return false;
        }

        // If we're going to use a window manager, symlink that onto our target.
        if (star->property("useWindowManager").toBool()) {
            if (!QFile::link(QString::fromLatin1("../hemera-window-manager.service"), QString::fromLatin1("%1hemera-window-manager.service").arg(targetRequires)) &&
                !m_settings.allowMultilink()) {
                setFinishedWithError(QStringLiteral("Could not create symlink %1hemera-window-manager.service!").arg(targetRequires), QString());
                return false;
            }
        }

        if (!QFile::link(QString::fromLatin1("../hemera-system-compositor-on-x.service"), QString::fromLatin1("%1hemera-system-compositor-on-x.service").arg(targetRequires)) &&
        !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1hemera-system-compositor-on-x.service!").arg(targetRequires), QString());
            return false;
        }

        environmentPlatform = QStringLiteral("wayland");

    } else if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::X11Display::staticMetaObject.className()) == 0) {
        outEnvFile << "# These variables determine the Display for the X11 display server\n";
        outEnvFile << QString::fromLatin1("DISPLAY=%1").arg(star->property("display").toString()) << '\n';
        environmentPlatform = QStringLiteral("X11");

        // We require xorg target to be up
        if (!QFile::link(QString::fromLatin1("../xorg.target"), QString::fromLatin1("%1xorg.target").arg(targetRequires)) && !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1xorg.target!").arg(targetRequires), QString());
            return false;
        }

        // If we're going to use a window manager, symlink that onto our target.
        if (star->property("useWindowManager").toBool()) {
            if (!QFile::link(QString::fromLatin1("../hemera-window-manager.service"), QString::fromLatin1("%1hemera-window-manager.service").arg(targetRequires)) &&
                !m_settings.allowMultilink()) {
                setFinishedWithError(QStringLiteral("Could not create symlink %1hemera-window-manager.service!").arg(targetRequires), QString());
                return false;
            }
        }
    } else if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::Appliance::staticMetaObject.className()) == 0 ||
        qstrcmp(displayMO->className(), Hemera::Qml::Settings::GUIAppliance::staticMetaObject.className()) == 0) {
        // Headless - nothing to do.
        environmentPlatform = QStringLiteral("headless");
    } else {
        setFinishedWithError(QStringLiteral("Wrong display type"), QString::fromLatin1("Trying to configure a display representing %1, but this type is not supported "
                                                                                        "by this version of Gravity Compiler.").arg(QLatin1String(displayMO->className())));
        return false;
    }
    std::cout << ".";

    // Environment
    if (!QFile::link(QString::fromLatin1("%1/%2").arg(StaticConfig::hemeraEnvironmentPath(), environmentPlatform),
        QString::fromLatin1("%1platform").arg(envDir)) && !m_settings.allowMultilink()) {
        setFinishedWithError(QStringLiteral("Could not create symlink %1platform!").arg(envDir), QString());
        return false;
    }

    destinationFile.write(serviceContent.toUtf8());
    destinationFile.flush();
    destinationFile.close();
    std::cout << ".";

    targetDestination.write(targetContent.toUtf8());
    targetDestination.flush();
    targetDestination.close();
    std::cout << ".";

    busPolicyDestination.write(busPolicyContent.toUtf8());
    busPolicyDestination.flush();
    busPolicyDestination.close();
    std::cout << ".";

    starBusPolicyDestination.write(starBusPolicyContent.toUtf8());
    starBusPolicyDestination.flush();
    starBusPolicyDestination.close();
    std::cout << ".";

    // List features for groups
    Hemera::Qml::Settings::OrbitFeatures::Features features =
            static_cast<Hemera::Qml::Settings::OrbitFeatures::Features>(star->property("features").toUInt());
    QStringList groups = featuresToGroups(features);
    if (qstrcmp(displayMO->className(), Hemera::Qml::Settings::WaylandDisplay::staticMetaObject.className()) == 0) {
        // Also weston-launch
        groups << QStringLiteral("weston-launch");
    }
    m_userList.insert(QString::fromLatin1("star-%1").arg(starName), groups);

    std::cout << ". Success!" << std::endl;
    return true;
}

void GravityCompileOperation::compileSandbox()
{
    // Let's get our target dir first
    QString userPath = m_settings.prefix() + QStringLiteral("/usr/lib/systemd/user/");
    QString systemPath = m_settings.prefix() + QStringLiteral("/lib/systemd/system/");
    QDir dir;
    QString name = m_name;

    // Is the name valid?
    if (!nameValidator.exactMatch(name)) {
        setFinishedWithError(QString::fromLatin1("The name %1 is not valid! Orbits must contain alphanumeric characters only.").arg(name), QString());
        return;
    }

    // Is the name reserved?
    if (reservedNames.contains(name)) {
        setFinishedWithError(QString::fromLatin1("The name %1 is reserved! Orbits can not have any of the following names: %2.")
                                                    .arg(name, reservedNames.join(QStringLiteral(", "))), QString());
        return;
    }

    // Is the name already taken?
    if (m_plugin->m_takenNames.contains(name)) {
        setFinishedWithError(QString::fromLatin1("There are two orbits named %1! Orbit names must be unique.").arg(name), QString());
        return;
    }

    m_plugin->m_takenNames.append(name);

    std::cout << "\tCompiling orbit " << name.toLatin1().constData();

    Hemera::Qml::Settings::OrbitFeatures::Features features =
        static_cast<Hemera::Qml::Settings::OrbitFeatures::Features>(m_object->property("features").toUInt());

    QString environment;
    if (!m_object->property("environment").toString().isEmpty()) {
        environment = QStringLiteral("Environment=") + m_object->property("environment").toString();
    }

    QStringList ownableDBusServices;
    bool isPlanet = false;
    QString userName;
    {
        // Rely on the private headers to avoid magic numbers
        using namespace Hemera::Qml::Settings;

        // List features for groups
        QStringList groups = featuresToGroups(features);

        // Check if we need to own planet
        if (features & OrbitFeatures::Planet) {
            isPlanet = true;
            ownableDBusServices << Hemera::Literals::literal(Hemera::Literals::DBus::planetService());
        }

        userName = QStringLiteral("orbit-%1").arg(name);
        m_userList.insert(userName, groups);
    }

    QString satelliteEnv;
    QString planetRuntimeCleanup;
    if (!isPlanet) {
        satelliteEnv = QStringLiteral("EnvironmentFile=-/run/hemera/stars/%I/planet/satelliteRuntimeEnv.conf");
    } else {
        planetRuntimeCleanup = QStringLiteral("ExecStopPost=/usr/libexec/hemera/cleanup-planet-satellite-runtime-dir %I star-%I");
    }

    QStringList applications;
    bool isOrbitalApplication = false;
    if (m_object->property("applications").isValid()) {
        applications = m_object->property("applications").toStringList();
    } else {
        applications << m_object->property("applicationId").toString();
        isOrbitalApplication = true;
    }
    ownableDBusServices << applications;
    std::cout << ".";

    // Get the payload
    QString serviceContent = QString::fromLatin1(payload(QStringLiteral("hemera-orbit-template.service.in")));
    QString targetContent = QString::fromLatin1(payload(QStringLiteral("hemera-orbit-template.target.in")));
    std::cout << ".";

    // Do the replace dance
    serviceContent.replace(QStringLiteral("@ORBIT@"), name);
    serviceContent.replace(QStringLiteral("@ENVIRONMENT@"), environment);
    serviceContent.replace(QStringLiteral("@HEMERA_GRAVITY_ENVIRONMENT_DIR@"), StaticConfig::hemeraEnvironmentPath());
    serviceContent.replace(QStringLiteral("@SATELLITE_ENVIRONMENT@"), satelliteEnv);
    serviceContent.replace(QStringLiteral("@DEBUGSUPPORT@"), QString());
    serviceContent.replace(QStringLiteral("@DEBUG@"), QString());
    serviceContent.replace(QStringLiteral("@EXIT_CLEANUP@"), planetRuntimeCleanup);

    targetContent.replace(QStringLiteral("@ORBIT@"), name);
    targetContent.replace(QStringLiteral("@ENVIRONMENT@"), environment);
    targetContent.replace(QStringLiteral("@DEBUG@"), QString());
    std::cout << ".";

    auto createFile = [this] (const QString &destination, const QString &content) -> bool {
        // Let's start by creating our files
        QFile destinationFile(destination);
        if (!destinationFile.open(QIODevice::WriteOnly)) {
            setFinishedWithError(QStringLiteral("Could not open ") + destination + QStringLiteral(" for writing!"), QString());
            return false;
        }

        destinationFile.write(content.toUtf8());
        destinationFile.flush();
        destinationFile.close();

        std::cout << ".";

        return true;
    };
    auto createRequiresWants = [this, userPath, applications, dir] (const QString &target, bool debugSupport = false) -> bool {
        // Now, let's put in the requires and wants
        QString targetRequires = userPath + target + QStringLiteral(".requires/");
        QString targetWants = userPath + target + QStringLiteral(".wants/");

        dir.mkpath(targetWants);
        dir.mkpath(targetRequires);

        for (const QString &application : applications) {
            if (!QFile::link(QString::fromLatin1("../%1.service").arg(debugSupport ? application + QStringLiteral("_debug") : application),
                                                                      QString::fromLatin1("%1%2.service").arg(targetRequires,
                                                                                            debugSupport ? application + QStringLiteral("_debug") : application)) &&
                !m_settings.allowMultilink()) {
                setFinishedWithError(QStringLiteral("Could not create symlink %1%2.service!").arg(targetRequires, application), QString());
                return false;
            }
        }
        std::cout << ".";

        // DBus (we really need it, for the session)
        if (!QFile::link(QStringLiteral("../dbus.service"), QString::fromLatin1("%1dbus.service").arg(targetRequires)) && !m_settings.allowMultilink()) {
            setFinishedWithError(QStringLiteral("Could not create symlink %1dbus.service!").arg(targetRequires), QString());
            return false;
        }
        std::cout << ".";
        return true;
    };
    auto createSettingsFile = [this] (const QString &orbitName, QString service) {
        // Let's load the settings file.
        QSettings sandbox(QString::fromLatin1("%1%2/%3.conf").arg(m_settings.prefix(), StaticConfig::configOrbitPath(), orbitName),
                        QSettings::NativeFormat);
        sandbox.clear();

        sandbox.beginGroup(QStringLiteral("Sandbox")); {
            std::cout << ".";
            sandbox.setValue(QStringLiteral("Name"), orbitName);
            sandbox.setValue(QStringLiteral("Service"), service.replace(QStringLiteral("@"), QStringLiteral("@%1")));
        } sandbox.endGroup();
    };

    if (!createFile(QString::fromLatin1("%1hemera-orbit-%2@.service").arg(systemPath, name), serviceContent)) {
        return;
    }
    if (!createFile(QString::fromLatin1("%1hemera-orbit-%2.target").arg(userPath, name), targetContent)) {
        return;
    }
    if (!createRequiresWants(QString::fromLatin1("hemera-orbit-%1.target").arg(name))) {
        return;
    }
    createSettingsFile(name, QString::fromLatin1("hemera-orbit-%1@.service").arg(name));

    // Write Hemera service file
    QJsonObject hemeraService;
    // It's an application.
    hemeraService.insert(QStringLiteral("type"), static_cast<int>(Hemera::ServiceInfo::ServiceType::Orbit));

    // Fill the common data
    hemeraService.insert(QStringLiteral("id"), name);
    hemeraService.insert(QStringLiteral("name"), name);

    // Fill the application-specific service data
    QJsonObject dataObject;
    dataObject.insert(QStringLiteral("applications"), QJsonArray::fromStringList(applications));
    dataObject.insert(QStringLiteral("hasDebugMode"), m_settings.debugSupport());
    dataObject.insert(QStringLiteral("isPlanet"), isPlanet);
    dataObject.insert(QStringLiteral("isOrbitalApplication"), isOrbitalApplication);

    hemeraService.insert(QStringLiteral("serviceData"), dataObject);

    QJsonDocument document(hemeraService);

    // We use Compact to save space, after all it has to be machine-parsed.
    createFile(QStringLiteral("%1%2/orbit-%3.hemeraservice").arg(m_settings.prefix(), StaticConfig::haFilesPath(), name),
               QLatin1String(document.toJson(QJsonDocument::Compact)));

    // Write DBus policy file.
    QString dbusPolicyContent = QString::fromLatin1(payload(QStringLiteral("star-bus-policy-orbit-template.conf.in")));
    QString fullOwnedNames;
    for (const QString &busName : ownableDBusServices) {
        fullOwnedNames += QStringLiteral("<allow own=\"%1\"/>\n").arg(busName);
    }
    dbusPolicyContent.replace(QStringLiteral("@ALLOW_OWN_MULTIPLE@"), fullOwnedNames);
    dbusPolicyContent.replace(QStringLiteral("@NAME@"), name);

    dir.mkpath(QString::fromLatin1("%1/etc/dbus-1/starbus.d/").arg(m_settings.prefix()));
    createFile(QStringLiteral("%1/etc/dbus-1/starbus.d/policy-orbit-%2.conf").arg(m_settings.prefix(), name), dbusPolicyContent);

    std::cout << ". Success!" << std::endl;

    // Do we need to compile debug support as well?
    if (m_settings.debugSupport()) {
        std::cout << "\tCompiling debug support for orbit " << name.toLatin1().constData();
        std::cout << ".";
        QString debugName = name + QStringLiteral("_debug");

        // Get the payload
        QString serviceDebugContent = QString::fromLatin1(payload(QStringLiteral("hemera-orbit-template.service.in")));
        QString targetDebugContent = QString::fromLatin1(payload(QStringLiteral("hemera-orbit-template.target.in")));
        std::cout << ".";

        // Do the replace dance
        serviceDebugContent.replace(QStringLiteral("@ORBIT@"), name);
        serviceDebugContent.replace(QStringLiteral("@ENVIRONMENT@"), environment);
        serviceDebugContent.replace(QStringLiteral("@HEMERA_GRAVITY_ENVIRONMENT_DIR@"), StaticConfig::hemeraEnvironmentPath());
        serviceDebugContent.replace(QStringLiteral("@DEBUGSUPPORT@"), QStringLiteral(" with debug support"));
        serviceDebugContent.replace(QStringLiteral("@DEBUG@"), QStringLiteral("_debug"));

        targetDebugContent.replace(QStringLiteral("@ORBIT@"), name);
        targetDebugContent.replace(QStringLiteral("@ENVIRONMENT@"), environment);
        targetDebugContent.replace(QStringLiteral("@DEBUG@"), QStringLiteral(", with debug support"));

        if (!createFile(QString::fromLatin1("%1hemera-orbit-%2@.service").arg(systemPath, debugName), serviceDebugContent)) {
            return;
        }
        if (!createFile(QString::fromLatin1("%1hemera-orbit-%2.target").arg(userPath, debugName), targetDebugContent)) {
            return;
        }
        if (!createRequiresWants(QString::fromLatin1("hemera-orbit-%1.target").arg(debugName), true)) {
            return;
        }
        createSettingsFile(debugName, QString::fromLatin1("hemera-orbit-%1@.service").arg(debugName));

        std::cout << ". Success!" << std::endl;
    }

    // Finalize users, as we created at least one for the sandbox.
    finalizeUsers();
}

