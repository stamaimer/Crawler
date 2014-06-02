#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "bestbuy.h"

#include "menu.h"
#include "merchandise.h"

#include "utils.h"
#include "inserter.h"

#include <QObject>

#include <QMutex>

#include <QVector>
#include <QString>
#include <QStringList>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QDebug>
#include <QElapsedTimer>

#include <QNetworkReply>

#define AMOUNT_OF_THREADS 30
#define MAX_REQUEST_COUNT 20

class Requester;

class JobScheduler : public QObject
{
    Q_OBJECT

    QVector<int> tids;//用于检查哪个线程没有死亡

    QVector<QString> completed;//记录已经发送过的URL

    Requester* requesters[AMOUNT_OF_THREADS];//请求线程数组

    QElapsedTimer timer;

    Utils utils;

    bool getJsonDoc(QNetworkReply*, BestBuy*, QJsonDocument*);

public:
    QMutex mutex;

    QVector<BestBuy*> bestbuys;

    QVector<Menu> menus;

    QVector<Merchandise> merchandises;

    QVector<QString> apikeys;

    Inserter* inserter;

    JobScheduler(QObject *parent = 0);

    bool getMenus(QNetworkReply*, BestBuy*);
    bool getMerchandises(QNetworkReply*, BestBuy*);

signals:

public slots:
    void finished(int);
};

#endif // JOBSCHEDULER_H
