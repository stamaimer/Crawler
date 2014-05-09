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

#define SLEEP_TIME 10

class Requester : public QThread
{
    Q_OBJECT

    static int final_menu_count;

    static int request_count;

    bool sleeped;

    int sleep_time;

    int tid;

    JobScheduler* job_scheduler;

public:
    Requester(int, JobScheduler*);

    void run();

signals:
    void finished(int);
};

#endif // REQUESTER_H
