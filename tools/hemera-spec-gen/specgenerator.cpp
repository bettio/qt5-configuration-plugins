#include "specgenerator.h"

#include <QtCore/QCoreApplication>
#include <QtCore/QDir>
#include <QtCore/QFile>

#include <QtQml/QQmlComponent>

#include <HemeraCore/Literals>
#include <HemeraCore/Global>

#include <HemeraQml/QmlEngine>

#include <qml/settings/hemeraqmlsimplecppapplication.h>

#include <pluginsconfig.h>

#include <iostream>

SpecGenerator::SpecGenerator(const QString& haFile, const QString& buildSystem,
                             const QString &manifestFile, QObject* parent)
    : BaseHaGenerator(haFile, true, parent)
    , m_buildSystem(buildSystem)
    , m_manifestFile(manifestFile)
{
}

SpecGenerator::~SpecGenerator()
{
}

void SpecGenerator::initImpl()
{
    connect(this, &SpecGenerator::ready, this, &SpecGenerator::writeSpecFile);

    if (!QFile::exists(m_manifestFile)) {
        setInitError(Hemera::Literals::literal(Hemera::Literals::Errors::notFound()),
                     tr("File %1 does not exist. Maybe you didn't install yet?").arg(m_manifestFile));
        return;
    }

    setThingsToDo(1);
    setReady();
}

void SpecGenerator::writeSpecFile()
{
    QString target = haObject()->property("applicationId").toString();
    target.remove(QLatin1Char('.'));

    bool isLocal = m_buildSystem.endsWith(QStringLiteral("-local"));

    auto replaceSpecPayload = [this, target, isLocal] (const QString &buildScript, const QString &installScript, const QString &rootOperationsScript, const QString &files, const QString &appDir) -> QString {
        QString groupName = haObject()->property("applicationId").toString().replace(QLatin1Char('.'), QLatin1Char('-'));
        QString appId = haObject()->property("applicationId").toString();
        QString appIdNoDots = haObject()->property("applicationId").toString().remove(QLatin1Char('.'));
        QString appOwnDir = QDir::cleanPath(StaticConfig::hemeraApplicationOwnResourceDir() +
                                            QDir::separator() + haObject()->property("applicationId").toString());

        QString macros;
        if (isLocal) {
            macros = QStringLiteral("%define debug_package %{nil}\n");
        }

        while (groupName.length() + 3 > 16) {
            if (!groupName.contains(QLatin1Char('-'))) {
                // trim the name
                groupName = groupName.left(16 - 3);
                break;
            }
            QStringList tmp = groupName.split(QLatin1Char('-'));
            tmp.removeFirst();
            groupName = tmp.join(QLatin1Char('-'));
        }

        // get dependency modules
        QStringList buildreqStrList;

        // be sure to have build system and hemera's sdk tools otherwise build will fail
        buildreqStrList << QString::fromLatin1("BuildRequires: %1").arg(m_buildSystem);
        buildreqStrList << QStringLiteral("BuildRequires: hemera-sdk-tools");

        // check what build system we're using for additional dependencies
        // and prep method
        QString prepStr;

        if (!isLocal) {
            prepStr += QString::fromLatin1("%setup -q -n %1-%{version}\n").arg(target);
        }

        // TODO qmake
        if (m_buildSystem == QStringLiteral("cmake")) {
            buildreqStrList << QStringLiteral("BuildRequires: common-cmake-modules");

            // prepare CMakeLists line
            prepStr += QStringLiteral(     "if [ ! -f CMakeLists.txt ]; then\n");
            prepStr += QStringLiteral(     "    if [ ! -f *.pro ]; then\n");
            prepStr += QString::fromLatin1("        %_hemera_configure %1.ha;\n\n").arg(haObject()->property("applicationId").toString());
            prepStr += QStringLiteral(     "    else\n");
            prepStr += QString::fromLatin1("        %_hemera_configure_with_pro %1.ha *.pro;\n\n").arg(haObject()->property("applicationId").toString());
            prepStr += QStringLiteral(     "    fi\n");
            prepStr += QStringLiteral(     "fi\n");
        }

        Hemera::Qml::Settings::QtModules::Modules qmlQtModules = static_cast<Hemera::Qml::Settings::QtModules::Modules>(haObject()->property("qtModules").toInt());
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Core) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Core)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Gui) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Gui)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Multimedia) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Multimedia)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Test) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Test)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Widgets) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Widgets)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Declarative) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Declarative)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::SerialPort) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5SerialPort)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Quick) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Quick)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Qml) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Qml)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::DBus) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5DBus)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::Network) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5Network)");
        }
        if (qmlQtModules & Hemera::Qml::Settings::QtModules::SystemInfo) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(Qt5SystemInfo)");
        }

        Hemera::Qml::Settings::HemeraModules::Modules qmlHemeraModules = static_cast<Hemera::Qml::Settings::HemeraModules::Modules>(haObject()->property("hemeraModules").toInt());
        if (qmlHemeraModules & Hemera::Qml::Settings::HemeraModules::Core) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HemeraQt5SDKCore)");
        }
        if (qmlHemeraModules & Hemera::Qml::Settings::HemeraModules::Gui) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HemeraQt5SDKGui)");
        }
        if (qmlHemeraModules & Hemera::Qml::Settings::HemeraModules::Multimedia) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HemeraQt5SDKMultimedia)");
        }
        if (qmlHemeraModules & Hemera::Qml::Settings::HemeraModules::Test) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HemeraQt5SDKTest)");
        }
        if (qmlHemeraModules & Hemera::Qml::Settings::HemeraModules::Qml) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HemeraQt5SDKQml)");
        }
        if (qmlHemeraModules & Hemera::Qml::Settings::HemeraModules::Widgets) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HemeraQt5SDKWidgets)");
        }

        Hemera::Qml::Settings::HyperspaceModules::Modules qmlHyperspaceModules =
                    static_cast<Hemera::Qml::Settings::HyperspaceModules::Modules>(haObject()->property("hyperspaceModules").toInt());

        // Sanity checks...
        if ((!haObject()->property("hyperspaceProducers").toStringList().isEmpty() || QQmlListReference(haObject(), "hyperspaceConsumers", haQmlEngine()).count() > 0) &&
            !(qmlHyperspaceModules & Hemera::Qml::Settings::HyperspaceModules::Module::ProducerConsumer)) {
            std::cout << "-- " << tr("WARNING: You requested to build a Hyperspace Producer and/or Consumer but did not enable the module! "
                                     "Enabling automatically...").toStdString() << std::endl;
            qmlHyperspaceModules |= Hemera::Qml::Settings::HyperspaceModules::Module::ProducerConsumer;
        }

        if (qmlHyperspaceModules & Hemera::Qml::Settings::HyperspaceModules::Core) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HyperspaceQt5Core)");
        }
        if (qmlHyperspaceModules & Hemera::Qml::Settings::HyperspaceModules::ProducerConsumer) {
            buildreqStrList << QStringLiteral("BuildRequires: pkgconfig(HyperspaceQt5ProducerConsumer)");
        }

        // Additional BuildRequires
        for (const QString &buildReq : haObject()->property("additionalPackageBuildRequires").toString()) {
            buildreqStrList << QStringLiteral("BuildRequires: %1").arg(buildReq);
        }

        // Additional Requires
        for (const QString &req : haObject()->property("additionalPackageRequires").toString()) {
            buildreqStrList << QStringLiteral("Requires: %1").arg(req);
        }

        return QString::fromLatin1(payload(QDir(StaticConfig::hemeraDataDir())
                                                .absoluteFilePath(QStringLiteral("hemera_app.spec.in"))))
                                  .replace(QStringLiteral("[macros]"), macros)
                                  .replace(QStringLiteral("[name]"), target)
                                  .replace(QStringLiteral("[version]"), haObject()->property("version").toString())
                                  .replace(QStringLiteral("[summary]"), QStringLiteral("Generated package for ") + target)
                                  .replace(QStringLiteral("[buildrequires]"), buildreqStrList.join(QLatin1Char('\n')))
                                  .replace(QStringLiteral("[description]"), haObject()->property("description").toString())
                                  .replace(QStringLiteral("[prep]"), prepStr)
                                  .replace(QStringLiteral("[buildscript]"), buildScript)
                                  .replace(QStringLiteral("[installscript]"), installScript)
                                  .replace(QStringLiteral("[rootoperations]"), rootOperationsScript)
                                  // Enable debug support on local specs
                                  .replace(QStringLiteral("[debugswitch]"), isLocal ? QStringLiteral("_debug") : QString())
                                  .replace(QStringLiteral("[files]"), files)
                                  .replace(QStringLiteral("[appres]"), appDir)
                                  .replace(QStringLiteral("[appownres]"), appOwnDir)
                                  .replace(QStringLiteral("[appid]"), appId)
                                  .replace(QStringLiteral("[appidnodots]"), appIdNoDots)
                                  .replace(QStringLiteral("[appiddashes]"), groupName);
    };

    QString appDir = QDir::cleanPath(StaticConfig::hemeraApplicationBaseResourceDir() +
                                     QDir::separator() + haObject()->property("applicationId").toString());
    QString buildScript;
    QString installScript;
    QStringList files;
    if (m_buildSystem == QStringLiteral("cmake")) {
        buildScript = QStringLiteral(
"mkdir build\n"
"cd build\n\n"
"%cmake ..\n"
"make %{?_smp_mflags}");
        installScript = QStringLiteral(
"cd build\n"
"%make_install");
        QFile file(m_manifestFile);
        file.open(QIODevice::ReadOnly);
        files = QString::fromLatin1(file.readAll()).split(QLatin1Char('\n'));
    } else if (m_buildSystem == QStringLiteral("qmake")) {
        buildScript = QStringLiteral(
"qmake\n"
"make %{?_smp_mflags}\n"
"# Gotta do this twice for the generator to work correctly\n"
"qmake\n"
"make %{?_smp_mflags}");
        installScript = QStringLiteral(
"%make_install");
    } else if (m_buildSystem == QStringLiteral("cmake-local")) {
        QFile file(m_manifestFile);
        file.open(QIODevice::ReadOnly);
        files = QString::fromLatin1(file.readAll()).split(QLatin1Char('\n'));
        installScript.append(QStringLiteral("# Local builds simply copy already built files over.\n"));

        // Let's just copy the files into the package
        QStringList dirs;
        for (const QString &file : files) {
            if (QFile::exists(file)) {
                // Retrieve its directory
                QString dir = file;
                dir.truncate(file.lastIndexOf(QLatin1Char('/')));

                // mkpath to the directory, but only once.
                if (!dirs.contains(dir)) {
                    installScript.append(QString::fromLatin1("mkdir -p %{buildroot}%1\n").arg(dir));
                    dirs.append(dir);
                }

                installScript.append(QString::fromLatin1("cp -a %1 %{buildroot}%1\n").arg(file));
            }
        }
    } else {
        qFatal("The buildsystem %s is not yet supported by this tool.", m_buildSystem.toLatin1().constData());
        return;
    }

    if (isLocal) {
        installScript.prepend(QStringLiteral("export DONT_STRIP=1\n\n"));
    }

    // Any root operations?
    QQmlListReference rootOperations(haObject(), "rootOperations", haQmlEngine());
    QString rootOperationsSymlinks;

    for (int i = 0; i < rootOperations.count(); ++i) {
        QObject *rootOperation = rootOperations.at(i);
        QString operationId = rootOperation->property("operationId").toString();

        // Symlink sockets
        rootOperationsSymlinks.append(QStringLiteral("ln -s ../hemera-rootoperation-%1.socket %{buildroot}%{_unitdir}/sockets.target.wants/hemera-rootoperation-%1.socket\n").arg(operationId));
        files.append(QStringLiteral("%{_unitdir}/sockets.target.wants/hemera-rootoperation-%1.socket").arg(operationId));
    }

    // Our scripts perform precompilation of python files to speed startup. If it's a py file, we have to handle this
    QStringList moreFiles;
    for (const QString &file : files) {
        if (file.endsWith(QStringLiteral(".py"))) {
            // Add pyc and pyo files
            moreFiles.append(QString::fromLatin1("%1c").arg(file));
            moreFiles.append(QString::fromLatin1("%1o").arg(file));
        }
    }

    // Add new files
    files.append(moreFiles);

    // Normalize files - everything which does not belong to the application directory needs a different attr
    for (QStringList::iterator i = files.begin(); i != files.end(); ++i) {
        if (!(*i).startsWith(appDir) && !(*i).isEmpty()) {
            (*i).prepend(QStringLiteral("%attr(-,root,root) "));
        } else if (!isLocal) {
            // We have to exclude the debug service from the equation
            if ((*i).endsWith(QString::fromLatin1("%1_debug.service").arg(haObject()->property("applicationId").toString()))) {
                (*i).prepend(QStringLiteral("%exclude "));
            }
        }
    }

    QString targetName = isLocal ? target + QStringLiteral("-local") : target;

    writeFile(QStringLiteral("ha-") + targetName + QStringLiteral(".spec"),
              replaceSpecPayload(buildScript, installScript, rootOperationsSymlinks, files.join(QLatin1Char('\n')), appDir).toLatin1());

    oneThingLessToDo();
}
