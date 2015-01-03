#include <QCoreApplication>

#include "scheduler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Scheduler scheduler;

    scheduler.exec();

    return a.exec();
}
