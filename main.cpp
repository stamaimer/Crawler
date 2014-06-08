#include <QCoreApplication>

#include "scheduler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Scheduler scheduler;

    scheduler.exec();

    exit(0);

    return a.exec();
}
