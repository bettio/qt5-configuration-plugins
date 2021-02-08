#include "hemeraqmlframebufferdisplay.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

FramebufferDisplay::FramebufferDisplay(QObject* parent)
    : Display(parent)
    , m_device(QStringLiteral("/dev/fb0"))
{
}

FramebufferDisplay::~FramebufferDisplay()
{
}

}

}

}
