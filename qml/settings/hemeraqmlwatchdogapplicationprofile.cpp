#include "hemeraqmlwatchdogapplicationprofile.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class WatchdogApplicationProfile::Private
{
public:
    Private() {}

    quint32 interval;
};


WatchdogApplicationProfile::WatchdogApplicationProfile(QObject* parent)
    : ApplicationProfile(parent)
    , d(new Private)
{
}


WatchdogApplicationProfile::~WatchdogApplicationProfile()
{
}


quint32 WatchdogApplicationProfile::interval() const
{
    return d->interval;
}


void WatchdogApplicationProfile::setInterval(quint32 interval)
{
    d->interval = interval;
}



};      // Hemera

};      // Qml

};      // Hemera
