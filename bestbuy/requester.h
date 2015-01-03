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
#define MAX_SLEEP_TIME 20

class Requester : public QThread
{
    Q_OBJECT

    static int send_count;

    int sleep_time;

    int tid;

    QString apikey;

    JobScheduler* job_scheduler;

public:
    Requester(int, QString, JobScheduler*);

    void run();

signals:
    void finished(int);

public slots:

};

#endif // REQUESTER_H
