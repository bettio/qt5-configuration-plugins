#include "pythonapplicationsgeneratorplugin.h"

#include <qml/settings/hemeraqmlpythonapplication.h>

#include <HemeraGenerators/BaseHaGenerator>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

#define PYTHON_BIN "/usr/bin/python3"

PythonApplicationsGeneratorPlugin::PythonApplicationsGeneratorPlugin(QObject* parent)
    : GeneratorPlugin(parent)
{

}

PythonApplicationsGeneratorPlugin::~PythonApplicationsGeneratorPlugin()
{

}

QString PythonApplicationsGeneratorPlugin::handlePayloadForClassName(const char* className,
                                                                  const QString& payloadType, const QVariantMap& arguments)
{
    if (qstrcmp(className, Hemera::Qml::Settings::PythonApplication::staticMetaObject.className())) {
        // None of our business
        return QString();
    }

    QString ret;

    // Handle payloadType
    if (payloadType == QStringLiteral("systemd-exec")) {
        ret = QString::fromLatin1("%1 %2%3%4%3%5").arg(QLatin1String(PYTHON_BIN), StaticConfig::hemeraApplicationBaseResourceDir(),
                                                       QDir::separator(), baseGenerator()->haObject()->property("applicationId").toString(),
                                                       baseGenerator()->haObject()->property("mainFile").toString());
    } else if (payloadType == QStringLiteral("systemd-exec-debug")) {
        ret = QString::fromLatin1("%1%2%3.sh").arg(StaticConfig::debugScriptsDir(), QDir::separator(),
                                                   baseGenerator()->haObject()->property("applicationId").toString());
    } else if (payloadType == QStringLiteral("systemd-additional-environment")) {
        ret = QString::fromLatin1("PYTHONPATH=%1%2%3").arg(StaticConfig::hemeraApplicationBaseResourceDir(),
                                                           QDir::separator(), baseGenerator()->haObject()->property("applicationId").toString());
    }

    return ret;
}

