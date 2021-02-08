#include "hemeraqmlsettingsmqtttransport.h"

#include "hemeraqmlsettingsnetworklisten.h"

namespace Hemera {
namespace Qml {
namespace Settings {

MQTTTransport::MQTTTransport(QObject* parent)
    : HyperspaceTransport(parent)
    , m_protocol(MQTTProtocol::TCP)
    , m_host(QStringLiteral("localhost"))
    , m_port(0)
    , m_publishQoS(MQTTQoS::AtLeastOnce)
    , m_subscribeQoS(MQTTQoS::ExactlyOnce)
    , m_cleanSession(true)
    , m_keepAlive(300)
    , m_useSSL(false)
    , m_serverCertificateVerification(true)
{

}

MQTTTransport::~MQTTTransport()
{

}

}
}
}
