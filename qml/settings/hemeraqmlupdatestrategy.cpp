#include "hemeraqmlupdatestrategy.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

UpdateStrategy::UpdateStrategy(QObject* parent)
    : QObject(parent)
{
}

UpdateStrategy::~UpdateStrategy()
{
}

ImageStoreUpdateStrategy::ImageStoreUpdateStrategy(QObject* parent)
    : UpdateStrategy(parent)
{
}

ImageStoreUpdateStrategy::~ImageStoreUpdateStrategy()
{
}

MassStorageUpdateStrategy::MassStorageUpdateStrategy(QObject* parent)
    : UpdateStrategy(parent)
    , m_automount(false)
{
}

MassStorageUpdateStrategy::~MassStorageUpdateStrategy()
{
}

AstarteUpdateStrategy::AstarteUpdateStrategy(QObject* parent)
    : UpdateStrategy(parent)
{
}

AstarteUpdateStrategy::~AstarteUpdateStrategy()
{
}

}
}
}
