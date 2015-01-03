#include "jobscheduler.h"
#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(QSslSocket::supportsSsl())
    {
        qDebug() << __TIME__ << __FUNCTION__ << "SSL enabled";
    }
    else
    {
        qDebug() << __TIME__ << __FUNCTION__ << "SSL unabled";
    }

    JobScheduler job_scheduler;

    return a.exec();
}


