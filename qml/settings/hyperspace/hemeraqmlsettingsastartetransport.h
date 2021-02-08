#ifndef HEMERA_QML_SETTINGS_ASTARTETRANSPORT_H
#define HEMERA_QML_SETTINGS_ASTARTETRANSPORT_H

#include "hemeraqmlsettingshyperspacetransport.h"

#include <QtCore/QUrl>
#include <QtQml/QQmlListProperty>

namespace Hemera {
namespace Qml {
namespace Settings {

class AstarteTransport : public HyperspaceTransport
{
    Q_OBJECT

    Q_PROPERTY(QUrl                                          endpoint                          MEMBER m_endpoint)
    Q_PROPERTY(QUrl                                          gateway                           MEMBER m_gateway)
    Q_PROPERTY(QString                                       signingKey                        MEMBER m_signingKey)
    Q_PROPERTY(QString                                       agentKey                          MEMBER m_agentKey)
    Q_PROPERTY(bool                                          rebootWhenConnectionFails         MEMBER m_rebootWhenConnectionFails)
    Q_PROPERTY(int                                           rebootDelayMinutes                MEMBER m_rebootDelayMinutes)

public:
    explicit AstarteTransport(QObject *parent = 0);
    virtual ~AstarteTransport();

private:
    QUrl m_endpoint;
    QUrl m_gateway;
    QString m_signingKey;
    QString m_agentKey;
    bool m_rebootWhenConnectionFails;
    int m_rebootDelayMinutes;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_ASTARTETRANSPORT_H
