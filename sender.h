#ifndef SENDER_H
#define SENDER_H

#include "jobscheduler.h"

#include <QObject>

#include <QThread>

#include <QEventLoop>

#include <QUrl>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class Sender : public QThread
{
    Q_OBJECT

    JobScheduler* job_scheduler;

    int thread_id;

public:
    explicit Sender(JobScheduler*);

    void start(int);//return type

    void run();//return type

signals:

public slots:

};

#endif // SENDER_H
