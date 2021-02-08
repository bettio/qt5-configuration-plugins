#ifndef HEMERA_QML_WATCHDOGAPPLICATIONPROFILE_H
#define HEMERA_QML_WATCHDOGAPPLICATIONPROFILE_H

#include "hemeraqmlapplicationprofile.h"

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * class used to set systemd values for the watchdog. Systemd vars it influences are:
 * ~ WatchdogSec
 */

class WatchdogApplicationProfile : public Hemera::Qml::Settings::ApplicationProfile
{
    Q_OBJECT

    Q_PROPERTY (quint32 interval        READ interval   WRITE setInterval)

public:
    explicit WatchdogApplicationProfile(QObject *parent = 0);
    virtual ~WatchdogApplicationProfile();

    quint32 interval() const;
    void setInterval(quint32 interval);

private:
    class Private;
    Private * const d;
};

};

};

};

#endif // HEMERA_QML_WATCHDOGAPPLICATIONPROFILE_H
