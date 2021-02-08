#include "hemeraqmlsettingsastartetransport.h"

#include "hemeraqmlsettingsnetworklisten.h"

namespace Hemera {
namespace Qml {
namespace Settings {

AstarteTransport::AstarteTransport(QObject* parent)
    : HyperspaceTransport(parent)
    , m_rebootWhenConnectionFails(false)
    , m_rebootDelayMinutes(600)
{
}

AstarteTransport::~AstarteTransport()
{
}

}
}
}
