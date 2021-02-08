#include <QtCore/QCoreApplication>
#include <QtCore/QDebug>
#include <QtCore/QStringList>

#include <HemeraCore/Operation>

#include "specgenerator.h"

#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    app.setApplicationName(SpecGenerator::tr("Hemera Spec Generator"));
    app.setOrganizationDomain(QStringLiteral("com.ispirata.Hemera"));
    app.setOrganizationName(QStringLiteral("Ispirata"));

    QStringList arguments = app.arguments();

    // Usage: hemera-spec-gen <hafile> <buildsystem> <manifestfile>
    if (arguments.count() != 4) {
        return 0;
    }

    SpecGenerator ag(arguments.at(1), arguments.at(2), arguments.at(3));

    Hemera::Operation *op = ag.init();

    QObject::connect(op, &Hemera::Operation::finished, [op] {
        if (op->isError()) {
            std::cerr << SpecGenerator::tr("SpecGenerator could not be initialized. %1: %2")
                         .arg(op->errorName(), op->errorMessage()).toStdString() << std::endl;
            QCoreApplication::instance()->exit(1);
        }
    });

    return app.exec();
}
