#include "hemeraqmlrestartonfailureapplicationprofile.h"


namespace Hemera {

namespace Qml {

namespace Settings {


class RestartOnFailureProfile::Private
{
public:
    Private() {}

    quint32 interval;
    quint32 maxFailuresPerInterval;
};


RestartOnFailureProfile::RestartOnFailureProfile(QObject *parent)
    : ApplicationProfile(parent)
    , d(new Private)
{
}


RestartOnFailureProfile::~RestartOnFailureProfile()
{
}


quint32 RestartOnFailureProfile::interval() const
{
    return d->interval;
}


quint32 RestartOnFailureProfile::maxFailuresPerInterval() const
{
    return d->maxFailuresPerInterval;
}


void RestartOnFailureProfile::setInterval(quint32 interval)
{
    d->interval = interval;
}


void RestartOnFailureProfile::setMaxFailuresPerInterval(quint32 limit)
{
    d->maxFailuresPerInterval = limit;
}


};      // Settings

};      // Qml

};      // Hemera
