#ifndef GRAVITYCOMPILERPLUGIN_H
#define GRAVITYCOMPILERPLUGIN_H

#include <HemeraGenerators/CompilerPlugin>

class GravityCompileOperation;
class GravityCompilerPlugin : public Hemera::Generators::CompilerPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(GravityCompilerPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.CompilerPlugin")
    Q_INTERFACES(Hemera::Generators::CompilerPlugin)

public:
    explicit GravityCompilerPlugin(QObject* parent = nullptr);
    virtual ~GravityCompilerPlugin();

    virtual Hemera::Operation *operationForClassName(const char* className, QObject* configurationObject,
                                                     const Hemera::Generators::CompilerConfiguration& compilerConfiguration) override final;

private:
    QStringList m_takenNames;

    friend class GravityCompileOperation;
};

#endif // GRAVITYCOMPILERPLUGIN_H
