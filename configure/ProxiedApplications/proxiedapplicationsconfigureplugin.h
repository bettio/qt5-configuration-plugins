/*
 *
 */

#ifndef PROXIEDAPPLICATIONSCONFIGUREPLUGIN_H
#define PROXIEDAPPLICATIONSCONFIGUREPLUGIN_H

#include <HemeraGenerators/ConfigurePlugin>

class ProxiedApplicationConfigurePlugin : public Hemera::Generators::ConfigurePlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(ProxiedApplicationConfigurePlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.ConfigurePlugin")
    Q_INTERFACES(Hemera::Generators::ConfigurePlugin)

public:
    ProxiedApplicationConfigurePlugin(QObject *parent = nullptr);
    virtual ~ProxiedApplicationConfigurePlugin();

    virtual bool handleClassName(const char* className) Q_DECL_OVERRIDE Q_DECL_FINAL;

private Q_SLOTS:
    void writeCMakeFile();
};

#endif // PROXIEDAPPLICATIONSCONFIGUREPLUGIN_H
