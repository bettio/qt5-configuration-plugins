/*
 *
 */

#ifndef QT5APPLICATIONSCONFIGUREPLUGIN_H
#define QT5APPLICATIONSCONFIGUREPLUGIN_H

#include <HemeraGenerators/ConfigurePlugin>

class Qt5ApplicationsConfigurePlugin : public Hemera::Generators::ConfigurePlugin
{
    Q_OBJECT
    Q_DISABLE_COPY(Qt5ApplicationsConfigurePlugin)
    Q_PLUGIN_METADATA(IID "com.ispirata.Hemera.Qt5SDK.Generators.ConfigurePlugin")
    Q_INTERFACES(Hemera::Generators::ConfigurePlugin)

public:
    enum class ApplicationType {
        NoType = 0,
        Headless,
        QtQuick2,
        QtWidgets
    };

    Qt5ApplicationsConfigurePlugin(QObject *parent = nullptr);
    virtual ~Qt5ApplicationsConfigurePlugin();

    virtual bool handleClassName(const char* className) Q_DECL_OVERRIDE Q_DECL_FINAL;

private Q_SLOTS:
    void writeSimpleQtQuick2ApplicationFiles();
    void writeSimpleQtQuick1ApplicationFiles();
    void writeSimpleCppApplicationFiles();

private:
    QString computeQmlUrl();
    void writeBaseMainCppFile(ApplicationType type, const QStringList &includes,
                              const QString &className, const QStringList &params);
    QString writeQt5RootWorkersFiles(const QString &applicationId);
};

#endif // QT5APPLICATIONSCONFIGUREPLUGIN_H
