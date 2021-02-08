/*
 *
 */

#include "hemeraqmlsimpleqtquick1application.h"

#include "hemeraqmlsimplecppapplication_p.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class SimpleQtQuick1ApplicationPrivate : public SimpleCppApplicationPrivate
{
public:
    SimpleQtQuick1ApplicationPrivate()
    : SimpleCppApplicationPrivate(QStringList() << QStringLiteral("main.cpp"),
                                  QtModules::Module::Core | QtModules::Module::DBus | QtModules::Module::Network | QtModules::Module::Gui | QtModules::Module::Declarative,
                                  HemeraModules::Module::Core | HemeraModules::Module::Gui | HemeraModules::Module::Qml | HemeraModules::Module::Widgets,
                                  HyperspaceModules::Module::Core | HyperspaceModules::Module::ProducerConsumer)
    {}

    QString mainQmlFile;
};

SimpleQtQuick1Application::SimpleQtQuick1Application(QObject* parent)
    : SimpleCppApplication(*new SimpleQtQuick1ApplicationPrivate, parent)
{
}

SimpleQtQuick1Application::~SimpleQtQuick1Application()
{
}

QString SimpleQtQuick1Application::mainQmlFile() const
{
    Q_D(const SimpleQtQuick1Application);
    return d->mainQmlFile;
}

void SimpleQtQuick1Application::setMainQmlFile(const QString& name)
{
    Q_D(SimpleQtQuick1Application);
    d->mainQmlFile = name;
}

}

}

}
