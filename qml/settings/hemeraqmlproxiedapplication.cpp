#include "hemeraqmlproxiedapplication.h"

#include "hemeraqmlapplication_p.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class ProxiedApplicationPrivate : public ApplicationPrivate
{
public:
    ProxiedApplicationPrivate()
        : ApplicationPrivate() {}

    QString command;
};

ProxiedApplication::ProxiedApplication(QObject* parent)
    : ProxiedApplication(*new ProxiedApplicationPrivate, parent)
{
}

ProxiedApplication::ProxiedApplication(ProxiedApplicationPrivate& dd, QObject* parent)
    : Application(dd, parent)
{
}

ProxiedApplication::~ProxiedApplication()
{
}

QString ProxiedApplication::command() const
{
    Q_D(const ProxiedApplication);
    return d->command;
}

void ProxiedApplication::setCommand(const QString& command)
{
    Q_D(ProxiedApplication);
    d->command = command;
}

}

}

}
