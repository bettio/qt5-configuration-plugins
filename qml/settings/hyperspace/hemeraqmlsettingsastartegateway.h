#ifndef HEMERA_QML_SETTINGS_ASTARTEGATEWAY_H
#define HEMERA_QML_SETTINGS_ASTARTEGATEWAY_H

#include "hemeraqmlsettingshyperspacetransport.h"

#include <QtCore/QUrl>
#include <QtQml/QQmlListProperty>

namespace Hemera {
namespace Qml {
namespace Settings {

class AstarteGateway : public HyperspaceTransport
{
    Q_OBJECT

    Q_PROPERTY(QUrl                                          endpoint                          MEMBER m_endpoint)
    Q_PROPERTY(QString                                       astarteBrokerHost                 MEMBER m_brokerHost)
    Q_PROPERTY(int                                           astarteBrokerPort                 MEMBER m_brokerPort)
    Q_PROPERTY(QStringList                                   topicMatchingRules                MEMBER m_topicMatchingRules)
    Q_PROPERTY(QString                                       signingKey                        MEMBER m_signingKey)

public:
    explicit AstarteGateway(QObject *parent = 0);
    virtual ~AstarteGateway();

private:
    QUrl m_endpoint;
    QString m_brokerHost;
    int m_brokerPort;
    QString m_signingKey;
    QStringList m_topicMatchingRules;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_ASTARTEGATEWAY_H
