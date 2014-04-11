#ifndef REQUESTER_H
#define REQUESTER_H

#include "jobscheduler.h"

#include <QThread>

#include <QEventLoop>

#include <QUrl>

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class Requester : QThread
{
    JobScheduler* job_scheduler;

    int tid;

public:
    Requester(JobScheduler*);

    void start(int);

    void run();
};

#endif // REQUESTER_H
