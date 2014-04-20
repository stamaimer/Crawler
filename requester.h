#ifndef REQUESTER_H
#define REQUESTER_H

#include "jobscheduler.h"

#include <QObject>
#include <QThread>

#include <QEventLoop>

#include <QUrl>

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class Requester : public QThread
{
    Q_OBJECT

    static int final_menu_count;

    bool sleeped;

    int tid;

    JobScheduler* job_scheduler;

public:
    Requester(int, JobScheduler*);

    void run();

signals:
    void finished(int);
};

#endif // REQUESTER_H
