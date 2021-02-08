#ifndef PYTHONAPPLICATIONSGENERATORPLUGIN_H
#define PYTHONAPPLICATIONSGENERATORPLUGIN_H

#include <HemeraGenerators/GeneratorPlugin>

class PythonApplicationsGeneratorPlugin : public Hemera::Generators::GeneratorPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(PythonApplicationsGeneratorPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.GeneratorPlugin")
    Q_INTERFACES(Hemera::Generators::GeneratorPlugin)

public:
    PythonApplicationsGeneratorPlugin(QObject *parent = nullptr);
    virtual ~PythonApplicationsGeneratorPlugin();

    virtual QString handlePayloadForClassName(const char* className, const QString& payloadType,
                                              const QVariantMap& arguments) override final;

private:
};

#endif // PYTHONAPPLICATIONSGENERATORPLUGIN_H
