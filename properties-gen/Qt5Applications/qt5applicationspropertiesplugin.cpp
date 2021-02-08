/*
 *
 */

#include "qt5applicationspropertiesplugin.h"

#include <qml/settings/hemeraqmlpythonapplication.h>

#include <HemeraGenerators/BasePropertiesGenerator>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QSet>

#include <pluginsconfig.h>

#include <iostream>

Qt5ApplicationsPropertiesPlugin::Qt5ApplicationsPropertiesPlugin(QObject* parent)
    : PropertiesPlugin(parent)
{

}

Qt5ApplicationsPropertiesPlugin::~Qt5ApplicationsPropertiesPlugin()
{

}

bool Qt5ApplicationsPropertiesPlugin::propertyGeneration(const char* className)
{
    if (baseGenerator()->sdk() != QStringLiteral("qt5")) {
        // Not our business.
        return false;
    }

    QString headerPayload;
    QString implPayload;

    QString header = QString::fromLatin1("%1.h").arg(baseGenerator()->outputFileName());
    QString impl = QString::fromLatin1("%1.cpp").arg(baseGenerator()->outputFileName());

    headerPayload = QString::fromLatin1(
"#ifndef APP_PROPS_HEMERA\n"
"#define APP_PROPS_HEMERA\n\n"
"#include <HemeraCore/ApplicationProperties>\n\n"
"class %1 : public Hemera::ApplicationProperties\n"
"{\n"
"public:\n"
"    %1();\n"
"};\n\n"
"#endif\n")
.arg(baseGenerator()->className());

        implPayload = QString::fromLatin1(
"#include <%1>\n\n"
"%2::%2()\n"
"    : Hemera::ApplicationProperties()\n"
"{\n"
"    setId(QStringLiteral(\"%3\"));\n"
"    setName(QStringLiteral(\"%4\"));\n"
"    setVersion(QStringLiteral(\"%5\"));\n"
"    setOrganization(QStringLiteral(\"%6\"));\n"
"}\n")
.arg(header).arg(baseGenerator()->className()).arg(baseGenerator()->haObject()->property("applicationId").toString())
.arg(baseGenerator()->haObject()->property("name").toString()).arg(baseGenerator()->haObject()->property("version").toString())
.arg(baseGenerator()->haObject()->property("organization").toString());

    // Now we're ready to push the payloads onto their correct destination
    baseGenerator()->writeFile(header, headerPayload.toUtf8());
    baseGenerator()->writeFile(impl, implPayload.toUtf8());

    return true;
}
