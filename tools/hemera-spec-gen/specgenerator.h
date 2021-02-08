#ifndef SPECGENERATOR_H
#define SPECGENERATOR_H

#include <QtCore/QObject>

#include <HemeraGenerators/BaseHaGenerator>

class QQmlEngine;

class SpecGenerator : public Hemera::Generators::BaseHaGenerator
{
    Q_OBJECT
public:
    explicit SpecGenerator(const QString &haFile, const QString & buildSystem,
                           const QString &manifestFile, QObject *parent = 0);
    virtual ~SpecGenerator();

public Q_SLOTS:
    void writeSpecFile();

protected:
    virtual void initImpl() Q_DECL_OVERRIDE;

private:
    QString m_buildSystem;
    QString m_manifestFile;
};

#endif // SPECGENERATOR_H
