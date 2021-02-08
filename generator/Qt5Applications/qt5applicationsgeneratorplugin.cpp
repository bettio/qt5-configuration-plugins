#include "qt5applicationsgeneratorplugin.h"

#include <qml/settings/hemeraqmlsimplecppapplication.h>
#include <qml/settings/hemeraqmlsimpleqmlapplication.h>
#include <qml/settings/hemeraqmlsimpleqtquick1application.h>

#include <HemeraGenerators/BaseHaGenerator>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

Qt5ApplicationsGeneratorPlugin::Qt5ApplicationsGeneratorPlugin(QObject* parent)
    : GeneratorPlugin(parent)
{

}

Qt5ApplicationsGeneratorPlugin::~Qt5ApplicationsGeneratorPlugin()
{

}

QString Qt5ApplicationsGeneratorPlugin::handlePayloadForClassName(const char* className,
                                                                  const QString& payloadType, const QVariantMap& arguments)
{
    if (qstrcmp(className, Hemera::Qml::Settings::SimpleCppApplication::staticMetaObject.className()) &&
        qstrcmp(className, Hemera::Qml::Settings::SimpleQmlApplication::staticMetaObject.className()) &&
        qstrcmp(className, Hemera::Qml::Settings::SimpleQtQuick1Application::staticMetaObject.className())) {
        // None of our business

        return QString();
    }

    QString ret;

    // Handle payloadType
    if (payloadType == QStringLiteral("systemd-exec")) {
        ret = QDir(StaticConfig::hemeraApplicationInstallDir()).filePath(arguments.value(QStringLiteral("target")).toString());
    } else if (payloadType == QStringLiteral("systemd-exec-debug")) {
        // cool.
        ret = QString::fromLatin1("%1%2%3.sh").arg(StaticConfig::debugScriptsDir(), QDir::separator(),
                                                   baseGenerator()->haObject()->property("applicationId").toString());
    } else if (payloadType == QStringLiteral("root-operation-systemd-exec")) {
        ret = QDir(StaticConfig::hemeraRootOperationInstallDir()).filePath(arguments.value(QStringLiteral("target")).toString());
    }

    return ret;
}

