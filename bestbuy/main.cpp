#include <QCoreApplication>
#include "jobscheduler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    JobScheduler job_scheduler;

    return a.exec();
}
