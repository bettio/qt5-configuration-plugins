#ifndef HEMERA_QML_SETTINGS_NETWORKLISTEN_H
#define HEMERA_QML_SETTINGS_NETWORKLISTEN_H

#include <QtCore/QObject>

namespace Hemera {
namespace Qml {
namespace Settings {

class NetworkListen : public QObject
{
    Q_OBJECT

    Q_PROPERTY(int port MEMBER m_port)
    Q_PROPERTY(QString ip MEMBER m_ip)
    Q_PROPERTY(QString interface MEMBER m_interface)
    Q_PROPERTY(bool ipv6Only MEMBER m_ipv6Only)
    Q_PROPERTY(bool secure MEMBER m_secure)

public:
    explicit NetworkListen(QObject *parent = nullptr);
    explicit NetworkListen(int port, const QString &ip, const QString &interface, bool ipv6Only, bool secure, QObject *parent = nullptr);
    virtual ~NetworkListen();

private:
    int m_port;
    QString m_ip;
    QString m_interface;
    bool m_ipv6Only;
    bool m_secure;
};
}
}
}

#endif // HEMERA_QML_SETTINGS_NETWORKLISTEN_H
