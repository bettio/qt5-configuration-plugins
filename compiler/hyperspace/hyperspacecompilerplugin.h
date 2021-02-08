#ifndef HYPERSPACECOMPILERPLUGIN_H
#define HYPERSPACECOMPILERPLUGIN_H

#include <HemeraGenerators/CompilerPlugin>

class HyperspaceCompilerPlugin : public Hemera::Generators::CompilerPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(HyperspaceCompilerPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.CompilerPlugin")
    Q_INTERFACES(Hemera::Generators::CompilerPlugin)

public:
    explicit HyperspaceCompilerPlugin(QObject* parent = nullptr);
    virtual ~HyperspaceCompilerPlugin();

    virtual Hemera::Operation* operationForClassName(const char* className, QObject* configurationObject,
                                                     const Hemera::Generators::CompilerConfiguration& compilerConfiguration);

private:
    friend class HyperspaceCompileOperation;
};

#endif // HYPERSPACECOMPILERPLUGIN_H
