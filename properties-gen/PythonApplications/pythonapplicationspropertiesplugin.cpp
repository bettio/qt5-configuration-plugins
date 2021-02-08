/*
 *
 */

#include "pythonapplicationspropertiesplugin.h"

#include <qml/settings/hemeraqmlpythonapplication.h>

#include <HemeraGenerators/BasePropertiesGenerator>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

PythonApplicationsPropertiesPlugin::PythonApplicationsPropertiesPlugin(QObject* parent)
    : PropertiesPlugin(parent)
{

}

PythonApplicationsPropertiesPlugin::~PythonApplicationsPropertiesPlugin()
{

}

bool PythonApplicationsPropertiesPlugin::propertyGeneration(const char* className)
{
    if (baseGenerator()->sdk() != QStringLiteral("python")) {
        // Not our business.
        return false;
    }

    QString pyPayload;

    QString pyfile = QString::fromLatin1("%1.py").arg(baseGenerator()->outputFileName());

    if (pyPayload.isEmpty()) {
        // No Plugins kicked in. Default generation.
        pyPayload = QString::fromLatin1(baseGenerator()->payload(QString::fromLatin1("%1/properties-gen/python-applications/properties_template.py.in")
                                            .arg(StaticConfig::hemeraDataDir())))
                                        .arg(baseGenerator()->className(), baseGenerator()->haObject()->property("applicationId").toString(),
                                             baseGenerator()->haObject()->property("name").toString(),
                                             baseGenerator()->haObject()->property("version").toString(),
                                             baseGenerator()->haObject()->property("organization").toString());
    }

    // Now we're ready to push the payloads onto their correct destination
    baseGenerator()->writeFile(pyfile, pyPayload.toUtf8());

    return true;
}
