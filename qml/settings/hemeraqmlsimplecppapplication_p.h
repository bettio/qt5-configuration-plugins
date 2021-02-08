/*
 *
 */

#include "hemeraqmlsimplecppapplication.h"

#include "hemeraqmlapplication_p.h"

namespace Hemera
{

namespace Qml
{

namespace Settings
{

class SimpleCppApplicationPrivate : public ApplicationPrivate
{
public:
    SimpleCppApplicationPrivate()
        : ApplicationPrivate()
        , qtModules(QtModules::Module::Core | QtModules::Module::DBus | QtModules::Module::Network)
        , hemeraModules(HemeraModules::Core)
        , hyperspaceModules(HyperspaceModules::NoModules) {}

    SimpleCppApplicationPrivate(const QStringList &sourceFiles
                            , QtModules::Modules qtModules
                            , HemeraModules::Modules hemeraModules
                            , HyperspaceModules::Modules hyperspaceModules)
        : ApplicationPrivate()
        , sourceFiles(sourceFiles)
        , qtModules(qtModules)
        , hemeraModules(hemeraModules)
        , hyperspaceModules(hyperspaceModules) {}

    QStringList sourceFiles;
    QStringList includePaths;
    QStringList libraries;

    QtModules::Modules qtModules;
    HemeraModules::Modules hemeraModules;
    HyperspaceModules::Modules hyperspaceModules;
};

}

}

}
