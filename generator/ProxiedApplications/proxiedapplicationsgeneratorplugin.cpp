#include "proxiedapplicationsgeneratorplugin.h"

#include <qml/settings/hemeraqmlproxiedapplication.h>

#include <HemeraGenerators/BaseHaGenerator>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

ProxiedApplicationsGeneratorPlugin::ProxiedApplicationsGeneratorPlugin(QObject* parent)
    : GeneratorPlugin(parent)
{

}

ProxiedApplicationsGeneratorPlugin::~ProxiedApplicationsGeneratorPlugin()
{

}

QString ProxiedApplicationsGeneratorPlugin::handlePayloadForClassName(const char* className,
                                                                  const QString& payloadType, const QVariantMap& arguments)
{
    if (qstrcmp(className, Hemera::Qml::Settings::ProxiedApplication::staticMetaObject.className())) {
        // None of our business
        return QString();
    }

    QString ret;

    // Handle payloadType
    if (payloadType == QStringLiteral("systemd-exec")) {
        ret = QString::fromLatin1("%1/hemera-application-launcher %2/%3 %4").arg(StaticConfig::hemeraToolsInstallDir(),
                                                                                 StaticConfig::haFilesPath(),
                                                                                 baseGenerator()->haFileName(),
                                                                                 baseGenerator()->haObject()->property("command").toString());
        ret.replace(QStringLiteral("@RESOURCE_DIR@"), QString::fromLatin1("%1/%2").arg(StaticConfig::hemeraApplicationBaseResourceDir(),
                                                                                       baseGenerator()->haObject()->property("applicationId").toString()));
    } else if (payloadType == QStringLiteral("systemd-exec-debug")) {
        // cool.
        ret = QString::fromLatin1("%1%2%3.sh").arg(StaticConfig::debugScriptsDir(), QDir::separator(),
                                                   baseGenerator()->haObject()->property("applicationId").toString());
    }

    return ret;
}

