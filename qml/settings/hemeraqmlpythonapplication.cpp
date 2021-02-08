#include "hemeraqmlpythonapplication.h"

#include "hemeraqmlapplication_p.h"

namespace Hemera
{
namespace Qml
{
namespace Settings
{

class PythonApplicationPrivate : public ApplicationPrivate
{
public:
    PythonApplicationPrivate()
        : ApplicationPrivate() {}
};

PythonApplication::PythonApplication(QObject* parent)
    : Application(*new PythonApplicationPrivate, parent)
    , m_generatePropertyClass(true)
    , m_propertyClassInstallationPath(QString())
{
}

PythonApplication::~PythonApplication()
{
}

}
}
}
