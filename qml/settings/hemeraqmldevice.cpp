#include "hemeraqmldevice.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class Device::Private
{
public:
    Private() : canRead(true), canWrite(false), canCreate(false) {}

    QString path;
    bool canRead;
    bool canWrite;
    bool canCreate;
};

Device::Device(QObject* parent)
    : QObject(parent)
    , d(new Private)
{
}

Device::~Device()
{
    delete d;
}

QString Device::path() const
{
    return d->path;
}

void Device::setPath(const QString& path)
{
    d->path = path;
}

bool Device::canRead() const
{
    return d->canRead;
}

void Device::setCanRead(bool canRead)
{
    d->canRead = canRead;
}

bool Device::canCreate() const
{
    return d->canCreate;
}

void Device::setCanCreate(bool canCreate)
{
    d->canCreate = canCreate;
}

bool Device::canWrite() const
{
    return d->canWrite;
}

void Device::setCanWrite(bool canWrite)
{
    d->canWrite = canWrite;
}

}

}

}
