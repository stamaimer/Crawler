#include <QCoreApplication>
#include "jobscheduler.h"

int main(int argc, char *argv[])
{
    QCoreApplication coreapp(argc, argv);

    JobScheduler job_scheduler;

    return coreapp.exec();
}
