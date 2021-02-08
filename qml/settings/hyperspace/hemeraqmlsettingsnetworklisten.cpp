#include "hemeraqmlsettingsnetworklisten.h"

namespace Hemera {
namespace Qml {
namespace Settings {

NetworkListen::NetworkListen(QObject* parent)
    : QObject(parent)
    , m_port(8080)
    , m_ip(QStringLiteral("0.0.0.0"))
    , m_interface(QStringLiteral("*"))
    , m_ipv6Only(false)
    , m_secure(true)
{
}

NetworkListen::NetworkListen(int port, const QString& ip, const QString& interface, bool ipv6Only, bool secure, QObject* parent)
    : QObject(parent)
    , m_port(port)
    , m_ip(ip)
    , m_interface(interface)
    , m_ipv6Only(ipv6Only)
    , m_secure(secure)
{
}

NetworkListen::~NetworkListen()
{
}

}
}
}
