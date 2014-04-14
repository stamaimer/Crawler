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

class Requester : public QThread
{
    static int count;

    int tid;

    JobScheduler* job_scheduler;

public:
    Requester(int, JobScheduler*);

    void run();
};

#endif // REQUESTER_H
