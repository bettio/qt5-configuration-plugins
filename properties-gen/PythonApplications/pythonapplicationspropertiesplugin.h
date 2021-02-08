/*
 *
 */

#ifndef PYTHONAPPLICATIONSPROPERTIESPLUGIN_H
#define PYTHONAPPLICATIONSPROPERTIESPLUGIN_H

#include <HemeraGenerators/PropertiesPlugin>

class PythonApplicationsPropertiesPlugin : public Hemera::Generators::PropertiesPlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(PythonApplicationsPropertiesPlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.PropertiesPlugin")
    Q_INTERFACES(Hemera::Generators::PropertiesPlugin)

public:
    PythonApplicationsPropertiesPlugin(QObject *parent = nullptr);
    virtual ~PythonApplicationsPropertiesPlugin();

    virtual bool propertyGeneration(const char* className) override final;
};

#endif // PROXIEDAPPLICATIONSPROPERTIESPLUGIN_H
