/*
 *
 */

#include "pythonapplicationsconfigureplugin.h"

#include <qml/settings/hemeraqmlpythonapplication.h>

#include <HemeraGenerators/BaseConfigure>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

PythonApplicationsConfigurePlugin::PythonApplicationsConfigurePlugin(QObject* parent)
    : ConfigurePlugin(parent)
{

}

PythonApplicationsConfigurePlugin::~PythonApplicationsConfigurePlugin()
{

}

bool PythonApplicationsConfigurePlugin::handleClassName(const char* className)
{
    // Let's see what to connect to:
    if (!qstrcmp(className, Hemera::Qml::Settings::PythonApplication::staticMetaObject.className())) {
        connect(baseConfigure(), &Hemera::AsyncInitObject::ready, this, &PythonApplicationsConfigurePlugin::writeCMakeFile);
        baseConfigure()->setThingsToDo(1);
        std::cout << "-- " << tr("Configuring target for a Hemera Python application.").toStdString() << std::endl;
        return true;
    }

    return false;
}

void PythonApplicationsConfigurePlugin::writeCMakeFile()
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

    QString cmakeData = QString::fromLatin1(baseConfigure()->payload(QString::fromLatin1("%1/configure/python-applications/CMakeLists.txt.in").arg(StaticConfig::hemeraDataDir())));

    {
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

        cmakeData.replace(QStringLiteral("@APPLICATION_BASE_NAME@"), execName.toLower());
        cmakeData.replace(QStringLiteral("@APPLICATION_ID@"), appId);
        cmakeData.replace(QStringLiteral("@APPLICATION_VERSION@"), baseConfigure()->haObject()->property("version").toString());
        cmakeData.replace(QStringLiteral("@INSTALL_RESOURCE_FILES@"), resourceFiles);

        // Generate property files?
        QString installProperties;
        if (baseConfigure()->haObject()->property("generatePropertyClass").toBool()) {
            installProperties = QString::fromLatin1("hemera_application_add_python_properties(${CMAKE_SOURCE_DIR}/%1.ha %2%3%4 GeneratedProperties generated_properties)\n")
                        .arg(appId, appResourcePath, QDir::separator(), baseConfigure()->haObject()->property("propertyClassInstallationPath").toString());
        }

        cmakeData.replace(QStringLiteral("@PYTHON_PROPERTIES@"), installProperties);
    }

    baseConfigure()->writeFile(cmakelists, cmakeData.toLatin1());
    baseConfigure()->oneThingLessToDo();
}
