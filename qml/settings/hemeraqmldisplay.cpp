#include "hemeraqmldisplay.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

Display::Display(QObject* parent)
    : Sandbox(parent)
    , m_powerSavingStrategies(PowerSavingStrategy::NoPowerSaving)
    , m_idleTime(3600)
{
    // By default, enable Video for displays.
    setProperty("features", OrbitFeatures::Video);
}

Display::~Display()
{
}

}

}

}
