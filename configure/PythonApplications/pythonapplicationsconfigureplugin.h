/*
 *
 */

#ifndef PYTHONAPPLICATIONSCONFIGUREPLUGIN_H
#define PYTHONAPPLICATIONSCONFIGUREPLUGIN_H

#include <HemeraGenerators/ConfigurePlugin>

class PythonApplicationsConfigurePlugin : public Hemera::Generators::ConfigurePlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(PythonApplicationsConfigurePlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.ConfigurePlugin")
    Q_INTERFACES(Hemera::Generators::ConfigurePlugin)

public:
    PythonApplicationsConfigurePlugin(QObject *parent = nullptr);
    virtual ~PythonApplicationsConfigurePlugin();

    virtual bool handleClassName(const char* className) Q_DECL_OVERRIDE Q_DECL_FINAL;

private Q_SLOTS:
    void writeCMakeFile();
};

#endif // PROXIEDAPPLICATIONSCONFIGUREPLUGIN_H
