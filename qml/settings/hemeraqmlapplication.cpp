#include "hemeraqmlapplication_p.h"

#include "hemeraqmlapplicationprofile.h"
#include "hemeraqmldevice.h"
#include "hemeraqmlrootoperation.h"
#include "hemeraqmlwatchdogapplicationprofile.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

Application::Application(QObject *parent)
    : Application(*new ApplicationPrivate, parent)
{
}

Application::Application(Hemera::Qml::Settings::ApplicationPrivate &dd, QObject *parent)
    : Sandbox(parent)
    , d_ptr(&dd)
    , m_generateOrbit(true)
{
}

Application::~Application()
{
    delete d_ptr;
}

QString Application::applicationId() const
{
    Q_D(const Application);
    return d->applicationId;
}

QQmlListProperty<ApplicationProfile> Application::applicationProfiles()
{
    return QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile>(this, 0,
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile> *list, ApplicationProfile *appProfile) {
                                                                Application *app = qobject_cast<Application*>(list->object);
                                                                if (appProfile) {
                                                                    app->d_func()->applicationProfiles.append(appProfile);
                                                                }
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile> *list) -> int {
                                                                return qobject_cast<Application*>(list->object)->d_func()->applicationProfiles.count();
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile> *list, int at) -> ApplicationProfile* {
                                                                return qobject_cast<Application*>(list->object)->d_func()->applicationProfiles.at(at);
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::ApplicationProfile> *list) {
                                                                Application *app = qobject_cast<Application*>(list->object);
                                                                qDeleteAll(app->d_func()->applicationProfiles);
                                                                app->d_func()->applicationProfiles.clear();
                                                            });
}

QQmlListProperty< RootOperation > Application::rootOperations()
{
    return QQmlListProperty<Hemera::Qml::Settings::RootOperation>(this, 0,
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::RootOperation> *list, RootOperation *rootOperation) {
                                                                Application *app = qobject_cast<Application*>(list->object);
                                                                if (rootOperation) {
                                                                    app->d_func()->rootOperations.append(rootOperation);
                                                                }
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::RootOperation> *list) -> int {
                                                                return qobject_cast<Application*>(list->object)->d_func()->rootOperations.count();
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::RootOperation> *list, int at) -> RootOperation* {
                                                                return qobject_cast<Application*>(list->object)->d_func()->rootOperations.at(at);
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::RootOperation> *list) {
                                                                Application *app = qobject_cast<Application*>(list->object);
                                                                qDeleteAll(app->d_func()->rootOperations);
                                                                app->d_func()->rootOperations.clear();
                                                            });
}

void Application::setApplicationId(const QString &id)
{
    Q_D(Application);
    d->applicationId = id;
}

QString Application::description() const
{
    Q_D(const Application);
    return d->description;
}

void Application::setDescription(const QString &description)
{
    Q_D(Application);
    d->description = description;
}

QString Application::organization() const
{
    Q_D(const Application);
    return d->organization;
}

void Application::setOrganization(const QString& organization)
{
    Q_D(Application);
    d->organization = organization;
}

QString Application::version() const
{
    Q_D(const Application);
    return d->version;
}

void Application::setVersion(const QString& version)
{
    Q_D(Application);
    d->version = version;
}

QString Application::icon() const
{
    Q_D(const Application);
    return d->icon;
}

void Application::setIcon(const QString& icon)
{
    Q_D(Application);
    d->icon = icon;
}

QStringList Application::resourceFiles() const
{
    Q_D(const Application);
    return d->resourceFiles;
}

void Application::setResourceFiles(const QStringList& resourceFiles)
{
    Q_D(Application);
    d->resourceFiles = resourceFiles;
}

QString Application::workingDirectory() const
{
    Q_D(const Application);
    return d->workingDirectory;
}

void Application::setWorkingDirectory(const QString& workingDirectory)
{
    Q_D(Application);
    d->workingDirectory = workingDirectory;
}

}

}

}

#include "moc_hemeraqmlapplication.cpp"
