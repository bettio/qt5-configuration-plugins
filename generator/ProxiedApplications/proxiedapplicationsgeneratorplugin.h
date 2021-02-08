#ifndef PROXIEDAPPLICATIONSGENERATORPLUGIN_H
#define PROXIEDAPPLICATIONSGENERATORPLUGIN_H

#include <HemeraGenerators/GeneratorPlugin>

class ProxiedApplicationsGeneratorPlugin : public Hemera::Generators::GeneratorPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(ProxiedApplicationsGeneratorPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.GeneratorPlugin")
    Q_INTERFACES(Hemera::Generators::GeneratorPlugin)

public:
    ProxiedApplicationsGeneratorPlugin(QObject *parent = nullptr);
    virtual ~ProxiedApplicationsGeneratorPlugin();

    virtual QString handlePayloadForClassName(const char* className, const QString& payloadType,
                                              const QVariantMap& arguments) override final;

private:
};

#endif // PROXIEDAPPLICATIONSGENERATORPLUGIN_H
