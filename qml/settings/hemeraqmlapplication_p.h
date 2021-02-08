#ifndef HEMERA_QML_HEMERAQMLAPPLICATION_P_H
#define HEMERA_QML_HEMERAQMLAPPLICATION_P_H

#include "hemeraqmlapplication.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class ApplicationPrivate
{
public:
    ApplicationPrivate() {}

    QString applicationId;
    QString description;
    QString version;
    QString icon;
    QString organization;
    QStringList resourceFiles;
    QString workingDirectory;

    QList<Hemera::Qml::Settings::ApplicationProfile*> applicationProfiles;
    QList<Hemera::Qml::Settings::Device*> devices;
    QList<Hemera::Qml::Settings::RootOperation*> rootOperations;
};

}

}

}

#endif
