#ifndef HEMERA_QML_RESTARTONFAILUREAPPLICATIONPROFILE_H
#define HEMERA_QML_RESTARTONFAILUREAPPLICATIONPROFILE_H

#include "hemeraqmlapplicationprofile.h"

namespace Hemera {

namespace Qml {

namespace Settings {

/**
 * systemd:
 * ~ Restart=on-failiure
 * ~ StartLimitInterval=interval
 * ~ StartLimitBurst=maxFailiursPerInterval
 * ~ StartLimitAction=reboot
 */

class RestartOnFailureProfile : public Hemera::Qml::Settings::ApplicationProfile
{
    Q_OBJECT

    Q_PROPERTY (quint32 interval                READ interval                   WRITE setInterval)
    Q_PROPERTY (quint32 maxFailuresPerInterval  READ maxFailuresPerInterval     WRITE setMaxFailuresPerInterval)

public:
    explicit RestartOnFailureProfile(QObject *parent = 0);
    virtual ~RestartOnFailureProfile();

    quint32 interval() const;
    quint32 maxFailuresPerInterval() const;

    void setInterval(quint32 interval);
    void setMaxFailuresPerInterval(quint32 limit);

private:
    class Private;
    Private * const d;
};

};

};

};

#endif // HEMERA_QML_RESTARTONFAILUREAPPLICATIONPROFILE_H
