#ifndef QT5APPLICATIONSGENERATORPLUGIN_H
#define QT5APPLICATIONSGENERATORPLUGIN_H

#include <HemeraGenerators/GeneratorPlugin>

class Qt5ApplicationsGeneratorPlugin : public Hemera::Generators::GeneratorPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(Qt5ApplicationsGeneratorPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.GeneratorPlugin")
    Q_INTERFACES(Hemera::Generators::GeneratorPlugin)

public:
    Qt5ApplicationsGeneratorPlugin(QObject *parent = nullptr);
    virtual ~Qt5ApplicationsGeneratorPlugin();

    virtual QString handlePayloadForClassName(const char* className, const QString& payloadType,
                                              const QVariantMap& arguments) override final;

private:
};

#endif // QT5APPLICATIONSGENERATORPLUGIN_H
