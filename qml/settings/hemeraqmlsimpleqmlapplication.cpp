/*
 *
 */

#include "hemeraqmlsimpleqmlapplication.h"

#include "hemeraqmlsimplecppapplication_p.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class SimpleQmlApplicationPrivate : public SimpleCppApplicationPrivate
{
public:
    SimpleQmlApplicationPrivate()
    : SimpleCppApplicationPrivate(QStringList() << QStringLiteral("main.cpp"),
                                  QtModules::Module::Core | QtModules::Module::DBus | QtModules::Module::Network | QtModules::Module::Gui | QtModules::Module::Quick,
                                  HemeraModules::Module::Core | HemeraModules::Module::Gui | HemeraModules::Module::Qml,
                                  HyperspaceModules::Module::Core | HyperspaceModules::Module::ProducerConsumer)
    {}

    QString mainQmlFile;
};

SimpleQmlApplication::SimpleQmlApplication(QObject* parent)
    : SimpleCppApplication(*new SimpleQmlApplicationPrivate, parent)
{
}

SimpleQmlApplication::~SimpleQmlApplication()
{
}

QString SimpleQmlApplication::mainQmlFile() const
{
    Q_D(const SimpleQmlApplication);
    return d->mainQmlFile;
}

void SimpleQmlApplication::setMainQmlFile(const QString& name)
{
    Q_D(SimpleQmlApplication);
    d->mainQmlFile = name;
}

}

}

}
