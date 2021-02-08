#include "hemeraqmlapplicationprofile.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class ApplicationProfile::Private
{
public:
    Private() {};
};

ApplicationProfile::ApplicationProfile(QObject *parent)
    : QObject(parent)
    , d(new Private)
{
}


ApplicationProfile::~ApplicationProfile()
{
    delete d;
}


};      // Settings

};      // Qml

};      // Hemera
