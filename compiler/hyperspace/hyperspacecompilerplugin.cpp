#include "hyperspacecompilerplugin.h"

#include "hyperspacecompileoperation.h"

HyperspaceCompilerPlugin::HyperspaceCompilerPlugin(QObject* parent)
    : CompilerPlugin(parent)
{

}

HyperspaceCompilerPlugin::~HyperspaceCompilerPlugin()
{

}

Hemera::Operation* HyperspaceCompilerPlugin::operationForClassName(const char* className, QObject* configurationObject,
                                                                   const Hemera::Generators::CompilerConfiguration& compilerConfiguration)
{
    // We compile only Hyperspace
    if (!qstrcmp(className, "Hemera::Qml::Settings::Hyperspace")) {
        return new HyperspaceCompileOperation(compilerConfiguration, configurationObject, baseCompiler()->qmlEngine(), this);
    }

    // We can't handle this
    return nullptr;
}
