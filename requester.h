#ifndef REQUESTER_H
#define REQUESTER_H

#include "jobscheduler.h"

#include <QObject>
#include <QThread>

#include <QEventLoop>

#include <QUrl>

#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#define SLEPP_TIME 10

class requester : public QThread
{
    Q_OBJECT

    static int send_count;

    int sleep_time;

    int tid;

    JobScheduler* job_scheduler;

public:
    requester(int, JobScheduler*);

    void run();

signals:
    void finished(int);

public slots:

};

#endif // REQUESTER_H
