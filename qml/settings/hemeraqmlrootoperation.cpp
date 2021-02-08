#include "hemeraqmlrootoperation.h"

namespace Hemera {
namespace Qml {
namespace Settings {

RootOperation::RootOperation(QObject* parent)
    : QObject(parent)
    , m_qtModules(QtModules::Module::Core | QtModules::Module::Network)
    , m_hemeraModules(HemeraModules::Core)
{
}

RootOperation::~RootOperation()
{
}

}
}
}
