#include "hemeraqmlx11display.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

X11Display::X11Display(QObject* parent)
    : Display(parent)
    , m_display(QStringLiteral(":0"))
    , m_useWindowManager(true)
{
}

X11Display::~X11Display()
{
}

}

}

}
