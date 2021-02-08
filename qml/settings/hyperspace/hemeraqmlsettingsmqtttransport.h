#ifndef HEMERA_QML_SETTINGS_MQTTTRANSPORT_H
#define HEMERA_QML_SETTINGS_MQTTTRANSPORT_H

#include "hemeraqmlsettingshyperspacetransport.h"

#include <QtQml/QQmlListProperty>

namespace Hemera {
namespace Qml {
namespace Settings {

class NetworkListen;

class MQTTQoS : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MQTTQoS)

public:
    enum QoS {
        AtMostOnce = 0,
        AtLeastOnce = 1,
        ExactlyOnce = 2
    };
    Q_ENUMS(QoS)
};

class MQTTProtocol : public QObject
{
    Q_OBJECT
    Q_DISABLE_COPY(MQTTProtocol)

public:
    enum Protocol {
        Unknown = 0,
        TCP = 1,
        SSL = 2,
        UDP = 3
    };
    Q_ENUMS(Protocol)
};

class MQTTTransport : public HyperspaceTransport
{
    Q_OBJECT

    Q_PROPERTY(Hemera::Qml::Settings::MQTTProtocol::Protocol protocol                          MEMBER m_protocol)
    Q_PROPERTY(QString                                       host                              MEMBER m_host)
    Q_PROPERTY(int                                           port                              MEMBER m_port)

    // Connection options
    Q_PROPERTY(Hemera::Qml::Settings::MQTTQoS::QoS           publishQoS                        MEMBER m_publishQoS)
    Q_PROPERTY(Hemera::Qml::Settings::MQTTQoS::QoS           subscribeQoS                      MEMBER m_subscribeQoS)

    Q_PROPERTY(bool                                          cleanSession                      MEMBER m_cleanSession)
    Q_PROPERTY(int                                           keepAlive                         MEMBER m_keepAlive)

    // SSL options
    Q_PROPERTY(QString                                       trustStore                        MEMBER m_trustStore)
    Q_PROPERTY(QString                                       keyStore                          MEMBER m_keyStore)
    Q_PROPERTY(QString                                       privateKey                        MEMBER m_privateKey)
    Q_PROPERTY(QString                                       privateKeyPassword                MEMBER m_privateKeyPassword)
    Q_PROPERTY(bool                                          serverCertificateVerification     MEMBER m_serverCertificateVerification)

public:
    explicit MQTTTransport(QObject *parent = 0);
    virtual ~MQTTTransport();

private:
    Hemera::Qml::Settings::MQTTProtocol::Protocol m_protocol;
    QString m_host;
    int m_port;

    Hemera::Qml::Settings::MQTTQoS::QoS m_publishQoS;
    Hemera::Qml::Settings::MQTTQoS::QoS m_subscribeQoS;

    bool m_cleanSession;
    int m_keepAlive;

    bool m_useSSL;
    QString m_trustStore;
    QString m_keyStore;
    QString m_privateKey;
    QString m_privateKeyPassword;
    bool m_serverCertificateVerification;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_MQTTTRANSPORT_H
