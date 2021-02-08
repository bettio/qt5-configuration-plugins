/*
 *
 */

#include "qt5applicationsconfigureplugin.h"

#include <qml/settings/hemeraqmlsimplecppapplication.h>
#include <qml/settings/hemeraqmlsimpleqmlapplication.h>
#include <qml/settings/hemeraqmlsimpleqtquick1application.h>

#include <HemeraGenerators/BaseConfigure>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

QStringList hemeraModules(Hemera::Qml::Settings::HemeraModules::Modules qmlHemeraModules)
{
    using namespace Hemera::Qml::Settings;

    QStringList hemeraModulesList;
    if (qmlHemeraModules & HemeraModules::Core) {
        hemeraModulesList << QStringLiteral("Core");
    }
    if (qmlHemeraModules & HemeraModules::Gui) {
        hemeraModulesList << QStringLiteral("Gui");
    }
    if (qmlHemeraModules & HemeraModules::Multimedia) {
        hemeraModulesList << QStringLiteral("Multimedia");
    }
    if (qmlHemeraModules & HemeraModules::Test) {
        hemeraModulesList << QStringLiteral("Test");
    }
    if (qmlHemeraModules & HemeraModules::Qml) {
        hemeraModulesList << QStringLiteral("Qml");
    }
    if (qmlHemeraModules & HemeraModules::SoftwareManagement) {
        hemeraModulesList << QStringLiteral("SoftwareManagement");
    }
    if (qmlHemeraModules & HemeraModules::Widgets) {
        hemeraModulesList << QStringLiteral("Widgets");
    }
    if (qmlHemeraModules & HemeraModules::WebClient) {
        hemeraModulesList << QStringLiteral("WebClient");
    }

    return hemeraModulesList;
}

QStringList hemeraNamespacedModules(Hemera::Qml::Settings::HemeraModules::Modules qmlHemeraModules)
{
    using namespace Hemera::Qml::Settings;

    QStringList hemeraNamespacedModulesList;
    for (const QString &module : hemeraModules(qmlHemeraModules)) {
        hemeraNamespacedModulesList << QStringLiteral("HemeraQt5SDK::") + module;
    }

    return hemeraNamespacedModulesList;
}

QStringList hyperspaceModules(Hemera::Qml::Settings::HyperspaceModules::Modules qmlHyperspaceModules)
{
    using namespace Hemera::Qml::Settings;

    QStringList hyperspaceModulesList;
    if (qmlHyperspaceModules & HyperspaceModules::Core) {
        hyperspaceModulesList << QStringLiteral("Core");
    }
    if (qmlHyperspaceModules & HyperspaceModules::ProducerConsumer) {
        hyperspaceModulesList << QStringLiteral("ProducerConsumer");
    }

    return hyperspaceModulesList;
}

QStringList hyperspaceNamespacedModules(Hemera::Qml::Settings::HyperspaceModules::Modules qmlHyperspaceModules)
{
    using namespace Hemera::Qml::Settings;

    QStringList hyperspaceNamespacedModulesList;
    for (const QString &module : hyperspaceModules(qmlHyperspaceModules)) {
        hyperspaceNamespacedModulesList << QStringLiteral("HyperspaceQt5::") + module;
    }

    return hyperspaceNamespacedModulesList;
}

QStringList qtModules(Hemera::Qml::Settings::QtModules::Modules qmlQtModules)
{
    using namespace Hemera::Qml::Settings;

    QStringList qtModulesList;
    if (qmlQtModules & QtModules::Core) {
        qtModulesList << QStringLiteral("Core");
    }
    if (qmlQtModules & QtModules::Gui) {
        qtModulesList << QStringLiteral("Gui");
    }
    if (qmlQtModules & QtModules::Multimedia) {
        qtModulesList << QStringLiteral("Multimedia");
    }
    if (qmlQtModules & QtModules::Test) {
        qtModulesList << QStringLiteral("Test");
    }
    if (qmlQtModules & QtModules::Widgets) {
        qtModulesList << QStringLiteral("Widgets");
    }
    if (qmlQtModules & QtModules::Declarative) {
        qtModulesList << QStringLiteral("Declarative");
    }
    if (qmlQtModules & QtModules::SerialPort) {
        qtModulesList << QStringLiteral("SerialPort");
    }
    if (qmlQtModules & QtModules::Quick) {
        qtModulesList << QStringLiteral("Quick");
    }
    if (qmlQtModules & QtModules::Qml) {
        qtModulesList << QStringLiteral("Qml");
    }
    if (qmlQtModules & QtModules::DBus) {
        qtModulesList << QStringLiteral("DBus");
    }
    if (qmlQtModules & QtModules::Network) {
        qtModulesList << QStringLiteral("Network");
    }
    if (qmlQtModules & QtModules::WebKit) {
        qtModulesList << QStringLiteral("WebKit");
    }
    if (qmlQtModules & QtModules::SystemInfo) {
        qtModulesList << QStringLiteral("SystemInfo");
    }
    if (qmlQtModules & QtModules::Compositor) {
        qtModulesList << QStringLiteral("Compositor");
    }
    if (qmlQtModules & QtModules::Sql) {
        qtModulesList << QStringLiteral("Sql");
    }

    return qtModulesList;
}

QStringList qtNamespacedModules(Hemera::Qml::Settings::QtModules::Modules qmlQtModules)
{
    using namespace Hemera::Qml::Settings;

    QStringList qtNamespacedModulesList;
    for (const QString &module : qtModules(qmlQtModules)) {
        qtNamespacedModulesList << QStringLiteral("Qt5::") + module;
    }

    return qtNamespacedModulesList;
}

QStringList hyperspaceProducers(const QStringList &producers, const QString &applicationBaseName)
{
    QStringList hyperspaceProducersList;
    for (const QString &producer : producers) {
        hyperspaceProducersList.append(QStringLiteral("hyperspace_add_qt5_producer(%1_SRCS %2)").arg(applicationBaseName, producer));
    }

    return hyperspaceProducersList;
}

QStringList hyperspaceConsumers(const QQmlListReference &consumers, const QString &applicationBaseName)
{
    QStringList hyperspaceConsumersList;
    for (int i = 0; i < consumers.count(); ++i) {
        hyperspaceConsumersList.append(QStringLiteral("hyperspace_add_qt5_consumer(%1_SRCS %2 %3 %4)")
                                        .arg(applicationBaseName, consumers.at(i)->property("interfaceFile").toString(),
                                             consumers.at(i)->property("header").toString(),
                                             consumers.at(i)->property("className").toString()));
    }

    return hyperspaceConsumersList;
}

Qt5ApplicationsConfigurePlugin::Qt5ApplicationsConfigurePlugin(QObject* parent): ConfigurePlugin(parent)
{

}

Qt5ApplicationsConfigurePlugin::~Qt5ApplicationsConfigurePlugin()
{

}

bool Qt5ApplicationsConfigurePlugin::handleClassName(const char* className)
{
    // Let's see what to connect to:
    if (!qstrcmp(className, Hemera::Qml::Settings::SimpleCppApplication::staticMetaObject.className())) {
        connect(baseConfigure(), &Hemera::AsyncInitObject::ready, this, &Qt5ApplicationsConfigurePlugin::writeSimpleCppApplicationFiles);
        baseConfigure()->setThingsToDo(1);
        std::cout << "-- " << tr("Configuring target for a generic C++/Qt5 application").toStdString() << std::endl;
        return true;
    } else if (!qstrcmp(className, Hemera::Qml::Settings::SimpleQmlApplication::staticMetaObject.className())) {
        connect(baseConfigure(), &Hemera::AsyncInitObject::ready, this, &Qt5ApplicationsConfigurePlugin::writeSimpleQtQuick2ApplicationFiles);
        baseConfigure()->setThingsToDo(2);
        std::cout << "-- " << tr("Configuring target for a Qt Quick 2 application").toStdString() << std::endl;
        return true;
    } else if (!qstrcmp(className, Hemera::Qml::Settings::SimpleQtQuick1Application::staticMetaObject.className())) {
        connect(baseConfigure(), &Hemera::AsyncInitObject::ready, this, &Qt5ApplicationsConfigurePlugin::writeSimpleQtQuick1ApplicationFiles);
        baseConfigure()->setThingsToDo(2);
        std::cout << "-- " << tr("Configuring target for a Qt Quick 1 application").toStdString() << std::endl;
        return true;
    }

    return false;
}

void Qt5ApplicationsConfigurePlugin::writeSimpleCppApplicationFiles()
{
    std::cout << "-- " << tr("Generating buildsystem files").toStdString() << std::endl;

    QString execName = baseConfigure()->haObject()->property("name").toString().toLower().replace(QStringLiteral(" "), QStringLiteral("-"));
    QString appId = baseConfigure()->haObject()->property("applicationId").toString();
    QString appResourcePath = StaticConfig::haBaseResourceDir() + QDir::separator() + appId;

    QString cmakelists;

    if (!baseConfigure()->destinationDir().isEmpty()) {
        // remove trailing slashes
        if (baseConfigure()->destinationDir().endsWith(QLatin1Char('/'))) {
            baseConfigure()->destinationDir().chop(1);
        }

        cmakelists = QString::fromLatin1("%1/CMakeLists.txt").arg(baseConfigure()->destinationDir());
    } else {
        cmakelists = QStringLiteral("CMakeLists.txt");
    }

    QString cmakeData = QString::fromLatin1(baseConfigure()->payload(QString::fromLatin1("%1/configure/qt5-applications/CMakeLists.txt.in").arg(StaticConfig::hemeraDataDir())));

    {
        using namespace Hemera::Qml::Settings;

        /*
         * Prepare source files.
         */
        QString sources = baseConfigure()->haObject()->property("sourceFiles").toStringList().join(QLatin1Char(' '));;

        // Icon
        QString serviceIcon;
        if (!baseConfigure()->haObject()->property("icon").toString().isEmpty()) {
            serviceIcon = QString::fromLatin1("hemera_service_add_icon(\"%1\" \"%2\")").arg(appId, baseConfigure()->haObject()->property("icon").toString());
        }

        QString includeDirectories = baseConfigure()->haObject()->property("includePaths").toStringList().isEmpty() ?
                                        QString() :
                                        QStringLiteral("include_directories(") +
                                        baseConfigure()->haObject()->property("includePaths").toStringList().join(QLatin1Char(' ')) +
                                        QLatin1Char(')');
        QString linkLibraries = baseConfigure()->haObject()->property("libraries").toStringList().isEmpty() ?
                                    QString() :
                                    QStringLiteral("target_link_libraries(") + execName + QLatin1Char(' ') +
                                    baseConfigure()->haObject()->property("libraries").toStringList().join(QLatin1Char(' ')) +
                                    QLatin1Char(')');

        QString resourceFiles;
        if (!baseConfigure()->haObject()->property("resourceFiles").toStringList().isEmpty()) {
            // Remove duplicates
            QStringList allFiles = baseConfigure()->haObject()->property("resourceFiles").toStringList().toSet().toList();

            QStringList dirs;
            QStringList dirFiles;
            QStringList baseFiles;

            // Handle resource files
            for (const QString &file : allFiles) {
                if (file.contains(QDir::separator())) {
                    /* Get the dir path. Due to how CMake works, we are interested *only*
                     * in the root directory, and not its subdirs. CMake's DIRECTORY is recursive
                     * over subdirs, and has a very different meaning when a subdir is instead selected.
                     * FILES_MATCHING PATTERN will do what's missing to match only selected files.
                     */
                    dirs.append(file.split(QDir::separator()).first());
                    dirFiles.append(file);
                } else {
                    baseFiles.append(file);
                }
            }

            // Remove duplicates from dirs. Otherwise, we would end up installing files multiple times.
            dirs = dirs.toSet().toList();

            if (!dirFiles.isEmpty() && !dirs.isEmpty()) {
                resourceFiles += QStringLiteral("install(DIRECTORY ") + dirs.join(QLatin1Char(' ')) +
                QStringLiteral("\n        DESTINATION ") + appResourcePath +
                QStringLiteral("\n        FILES_MATCHING PATTERN \"") +
                dirFiles.join(QLatin1String("\" PATTERN \"")) + QStringLiteral("\")\n");
            }
            if (!baseFiles.isEmpty()) {
                resourceFiles += QStringLiteral("install(FILES ") + baseFiles.join(QLatin1Char(' ')) +
                QStringLiteral("\n        DESTINATION ") + appResourcePath + QLatin1Char(')');
            }
        }

        HemeraModules::Modules qmlHemeraModules = static_cast<HemeraModules::Modules>(baseConfigure()->haObject()->property("hemeraModules").toInt());
        HyperspaceModules::Modules qmlHyperspaceModules = static_cast<HyperspaceModules::Modules>(baseConfigure()->haObject()->property("hyperspaceModules").toInt());
        QtModules::Modules qmlQtModules = static_cast<QtModules::Modules>(baseConfigure()->haObject()->property("qtModules").toInt());

        // Hyperspace Producers/Consumers
        QStringList hyperspacePCList = hyperspaceProducers(baseConfigure()->haObject()->property("hyperspaceProducers").toStringList(), execName.toLower());
        QQmlListReference consumersReference(baseConfigure()->haObject(), "hyperspaceConsumers", baseConfigure()->haQmlEngine());
        hyperspacePCList.append(hyperspaceConsumers(consumersReference, execName.toLower()));

        // Sanity checks...
        if (!hyperspacePCList.isEmpty() && !(qmlHyperspaceModules & HyperspaceModules::Module::ProducerConsumer)) {
            std::cout << "-- " << tr("WARNING: You requested to build a Hyperspace Producer and/or Consumer but did not enable the module! "
                                     "Enabling automatically...").toStdString() << std::endl;
            qmlHyperspaceModules |= HyperspaceModules::Module::ProducerConsumer;
        }

        cmakeData.replace(QStringLiteral("@APPLICATION_BASE_NAME@"), execName.toLower());
        cmakeData.replace(QStringLiteral("@APPLICATION_ID@"), appId);
        cmakeData.replace(QStringLiteral("@APPLICATION_VERSION@"), baseConfigure()->haObject()->property("version").toString());
        cmakeData.replace(QStringLiteral("@QT_MODULES@"), qtModules(qmlQtModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@QT_NAMESPACED_MODULES@"), qtNamespacedModules(qmlQtModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@HEMERA_MODULES@"), hemeraModules(qmlHemeraModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@HEMERA_NAMESPACED_MODULES@"), hemeraNamespacedModules(qmlHemeraModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@HYPERSPACE_MODULES@"), hyperspaceModules(qmlHyperspaceModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@HYPERSPACE_NAMESPACED_MODULES@"), hyperspaceNamespacedModules(qmlHyperspaceModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@INCLUDE_DIRECTORIES@"), includeDirectories);
        cmakeData.replace(QStringLiteral("@SOURCES@"), sources);
        cmakeData.replace(QStringLiteral("@HYPERSPACE_PRODUCERS_CONSUMERS@"), hyperspacePCList.join(QLatin1Char('\n')));
        cmakeData.replace(QStringLiteral("@SERVICE_ICON@"), serviceIcon);
        cmakeData.replace(QStringLiteral("@LINK_LIBRARIES@"), linkLibraries);
        cmakeData.replace(QStringLiteral("@INSTALL_RESOURCE_FILES@"), resourceFiles);

        // Root operations
        cmakeData.replace(QStringLiteral("@ROOT_OPERATIONS@"), writeQt5RootWorkersFiles(appId));
    }

    baseConfigure()->writeFile(cmakelists, cmakeData.toLatin1());
    baseConfigure()->oneThingLessToDo();
}

void Qt5ApplicationsConfigurePlugin::writeBaseMainCppFile(ApplicationType type, const QStringList &includes,
                                                          const QString &className, const QStringList &params)
{
    std::cout << "-- " << tr("Generating C++ support files").toStdString() << std::endl;

    QString includePaths;
    for (const QString &include : includes) {
        includePaths.append(QString::fromLatin1("#include <%1>\n").arg(include));
    }

    QString parameters = params.isEmpty() ? QString() : QStringLiteral(", ");
    parameters.append(params.join(QStringLiteral(", ")));

    QString appId = baseConfigure()->haObject()->property("applicationId").toString();

    QString maincppData;

    switch (type) {
        case ApplicationType::Headless:
            maincppData = QString::fromLatin1(baseConfigure()->payload(QString::fromLatin1("%1/configure/qt5-applications/main-headless.cpp.in").arg(StaticConfig::hemeraDataDir())));
            break;
        case ApplicationType::QtQuick2:
            maincppData = QString::fromLatin1(baseConfigure()->payload(QString::fromLatin1("%1/configure/qt5-applications/main-quick2.cpp.in").arg(StaticConfig::hemeraDataDir())));
            break;
        case ApplicationType::QtWidgets:
            maincppData = QString::fromLatin1(baseConfigure()->payload(QString::fromLatin1("%1/configure/qt5-applications/main-widgets.cpp.in").arg(StaticConfig::hemeraDataDir())));
            break;
        default:
            baseConfigure()->terminateWithError(tr("The base cpp file refers to an unknown application type."));
    }

    maincppData.replace(QStringLiteral("@INCLUDES@"), includePaths);
    maincppData.replace(QStringLiteral("@BASE_CLASS@"), className);
    maincppData.replace(QStringLiteral("@PARAMS@"), parameters);

    QString maincpp;
    if (!baseConfigure()->destinationDir().isEmpty()) {
        // remove trailing slashes
        if (baseConfigure()->destinationDir().endsWith(QLatin1Char('/'))) {
            baseConfigure()->destinationDir().chop(1);
        }

        maincpp = QString::fromLatin1("%1/main.cpp").arg(baseConfigure()->destinationDir());
    } else {
        maincpp = QStringLiteral("main.cpp");
    }

    baseConfigure()->writeFile(maincpp, maincppData.toLatin1());
    baseConfigure()->oneThingLessToDo();
}

QString Qt5ApplicationsConfigurePlugin::writeQt5RootWorkersFiles(const QString &applicationId)
{
    QString cmakeAdditions;
    QQmlListReference rootOperations(baseConfigure()->haObject(), "rootOperations", baseConfigure()->haQmlEngine());

    for (int i = 0; i < rootOperations.count(); ++i) {
        QObject *rootOperation = rootOperations.at(i);

        QString operationId = rootOperation->property("operationId").toString();
        std::cout << "-- " << tr("Generating files for root operation").toStdString() << " " << operationId.toLatin1().constData() << std::endl;

        QString cmakelists;

        if (!baseConfigure()->destinationDir().isEmpty()) {
            // remove trailing slashes
            if (baseConfigure()->destinationDir().endsWith(QLatin1Char('/'))) {
                baseConfigure()->destinationDir().chop(1);
            }

            cmakelists = QStringLiteral("%1/hro-%2.cmake").arg(baseConfigure()->destinationDir(), operationId);
        } else {
            cmakelists = QStringLiteral("hro-%1.cmake").arg(operationId);
        }

        QString cmakeData = QString::fromLatin1(baseConfigure()->payload(QString::fromLatin1("%1/configure/qt5-applications/RootOperation.cmake.in").arg(StaticConfig::hemeraDataDir())));

        /*
         * Prepare source files.
         */
        QString sources = rootOperation->property("sourceFiles").toStringList().join(QLatin1Char(' '));;

        QString includeDirectories = rootOperation->property("includePaths").toStringList().isEmpty() ?
                                        QString() :
                                        QStringLiteral("include_directories(") +
                                        rootOperation->property("includePaths").toStringList().join(QLatin1Char(' ')) +
                                        QLatin1Char(')');
        QString linkLibraries = rootOperation->property("libraries").toStringList().isEmpty() ?
                                    QString() :
                                    QStringLiteral("target_link_libraries(hemera-rootoperation-") + operationId + QLatin1Char(' ') +
                                    rootOperation->property("libraries").toStringList().join(QLatin1Char(' ')) +
                                    QLatin1Char(')');

        using namespace Hemera::Qml::Settings;

        HemeraModules::Modules qmlHemeraModules = static_cast<HemeraModules::Modules>(rootOperation->property("hemeraModules").toInt());
        QtModules::Modules qmlQtModules = static_cast<QtModules::Modules>(rootOperation->property("qtModules").toInt());

        cmakeData.replace(QStringLiteral("@APPLICATION_ID@"), applicationId);
        cmakeData.replace(QStringLiteral("@OPERATION_ID@"), operationId);
        cmakeData.replace(QStringLiteral("@QT_MODULES@"), qtModules(qmlQtModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@QT_NAMESPACED_MODULES@"), qtNamespacedModules(qmlQtModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@HEMERA_MODULES@"), hemeraModules(qmlHemeraModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@HEMERA_NAMESPACED_MODULES@"), hemeraNamespacedModules(qmlHemeraModules).join(QLatin1Char(' ')));
        cmakeData.replace(QStringLiteral("@INCLUDE_DIRECTORIES@"), includeDirectories);
        cmakeData.replace(QStringLiteral("@SOURCES@"), sources);
        cmakeData.replace(QStringLiteral("@LINK_LIBRARIES@"), linkLibraries);

        baseConfigure()->writeFile(cmakelists, cmakeData.toLatin1());

        cmakeAdditions.append(QStringLiteral("include(hro-%1.cmake)\n").arg(operationId));
    }

    return cmakeAdditions;
}

QString Qt5ApplicationsConfigurePlugin::computeQmlUrl()
{
    QString mainQmlFile = baseConfigure()->haObject()->property("mainQmlFile").toString();
    if (mainQmlFile.startsWith(QStringLiteral("http://")) || mainQmlFile.startsWith(QStringLiteral("ftp://"))) {
        return QStringLiteral("QUrl(QStringLiteral(\"") + mainQmlFile + QStringLiteral("\"))");
    } else {
        return QStringLiteral("QUrl(QStringLiteral(\"resource:///") +  mainQmlFile + QStringLiteral("\"))");
    }
}

void Qt5ApplicationsConfigurePlugin::writeSimpleQtQuick2ApplicationFiles()
{
    writeSimpleCppApplicationFiles();
    writeBaseMainCppFile(ApplicationType::QtQuick2,
                         QStringList() << QStringLiteral("HemeraGui/SimpleQmlGuiApplication") << QStringLiteral("QtCore/QUrl"),
                         QStringLiteral("Hemera::SimpleQmlGuiApplication"), QStringList() << computeQmlUrl());
}

void Qt5ApplicationsConfigurePlugin::writeSimpleQtQuick1ApplicationFiles()
{
    writeSimpleCppApplicationFiles();
    writeBaseMainCppFile(ApplicationType::QtWidgets,
                         QStringList() << QStringLiteral("HemeraWidgets/SimpleQtQuick1Application") << QStringLiteral("QtCore/QUrl"),
                         QStringLiteral("Hemera::SimpleQtQuick1Application"), QStringList() << computeQmlUrl());
}
