#include "hemeraqmlsettingsastartegateway.h"

namespace Hemera {
namespace Qml {
namespace Settings {

AstarteGateway::AstarteGateway(QObject* parent)
    : HyperspaceTransport(parent)
    , m_brokerPort(1883)
{
}

AstarteGateway::~AstarteGateway()
{
}

}
}
}
