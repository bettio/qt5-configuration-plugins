/*
 *
 */

#include "hemeraqmlsimplecppapplication_p.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

SimpleCppApplication::SimpleCppApplication(QObject* parent)
    : Application(*new SimpleCppApplicationPrivate, parent)
{
}

SimpleCppApplication::SimpleCppApplication(SimpleCppApplicationPrivate& dd, QObject* parent)
    : Application(dd, parent)
{
}

SimpleCppApplication::~SimpleCppApplication()
{
}

Hemera::Qml::Settings::HemeraModules::Modules SimpleCppApplication::hemeraModules() const
{
    Q_D(const SimpleCppApplication);
    return d->hemeraModules;
}

HyperspaceModules::Modules SimpleCppApplication::hyperspaceModules() const
{
    Q_D(const SimpleCppApplication);
    return d->hyperspaceModules;
}

void SimpleCppApplication::setHemeraModules(Hemera::Qml::Settings::HemeraModules::Modules modules)
{
    Q_D(SimpleCppApplication);
    d->hemeraModules = modules;
}

void SimpleCppApplication::setHyperspaceModules(Hemera::Qml::Settings::HyperspaceModules::Modules modules)
{
    Q_D(SimpleCppApplication);
    d->hyperspaceModules = modules;
}

QStringList SimpleCppApplication::includePaths() const
{
    Q_D(const SimpleCppApplication);
    return d->includePaths;
}

void SimpleCppApplication::setIncludePaths(const QStringList& includePaths)
{
    Q_D(SimpleCppApplication);
    d->includePaths = includePaths;
}

QStringList SimpleCppApplication::libraries() const
{
    Q_D(const SimpleCppApplication);
    return d->libraries;
}

void SimpleCppApplication::setLibraries(const QStringList& libraries)
{
    Q_D(SimpleCppApplication);
    d->libraries = libraries;
}

Hemera::Qml::Settings::QtModules::Modules SimpleCppApplication::qtModules() const
{
    Q_D(const SimpleCppApplication);
    return d->qtModules;
}

void SimpleCppApplication::setQtModules(Hemera::Qml::Settings::QtModules::Modules modules)
{
    Q_D(SimpleCppApplication);
    d->qtModules = modules;
}

QStringList SimpleCppApplication::sourceFiles() const
{
    Q_D(const SimpleCppApplication);
    return d->sourceFiles;
}

void SimpleCppApplication::setSourceFiles(const QStringList& sourceFiles)
{
    Q_D(SimpleCppApplication);
    d->sourceFiles = sourceFiles;
}

QQmlListProperty<HyperspaceQt5Consumer> SimpleCppApplication::hyperspaceConsumers()
{
    return QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer>(this, 0,
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer> *list, HyperspaceQt5Consumer *consumer) {
                                                                SimpleCppApplication *app = qobject_cast<SimpleCppApplication*>(list->object);
                                                                if (consumer) {
                                                                    app->m_hyperspaceConsumers.append(consumer);
                                                                }
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer> *list) -> int {
                                                                return qobject_cast<SimpleCppApplication*>(list->object)->m_hyperspaceConsumers.count();
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer> *list, int at) -> HyperspaceQt5Consumer* {
                                                                return qobject_cast<SimpleCppApplication*>(list->object)->m_hyperspaceConsumers.at(at);
                                                            },
                                                            [] (QQmlListProperty<Hemera::Qml::Settings::HyperspaceQt5Consumer> *list) {
                                                                SimpleCppApplication *app = qobject_cast<SimpleCppApplication*>(list->object);
                                                                qDeleteAll(app->m_hyperspaceConsumers);
                                                                app->m_hyperspaceConsumers.clear();
                                                            });
}

}

}

}
