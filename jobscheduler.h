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

#define AMOUNT_OF_THREADS 200
#define MAX_REQUEST_COUNT 20

class Requester;

class JobScheduler : public QObject
{
    Q_OBJECT

    QVector<int> tids;//用于检查哪个线程没有死亡

    QVector<Menu> menus;

    QVector<QString> completed;//记录已经发送过的URL

    Requester* requesters[AMOUNT_OF_THREADS];//请求线程数组

    Utils utils;

    bool getJsonDoc(QNetworkReply*, BestBuy*, QJsonDocument*);

public:
    QMutex mutex;

    QElapsedTimer timer;

    QVector<BestBuy*> bestbuys;

    QVector<Merchandise> merchandises;

    Inserter* inserter;

    explicit JobScheduler(QObject *parent = 0);

    bool getMenus(QNetworkReply*, BestBuy*);
    bool getMerchandises(QNetworkReply*, BestBuy*);

signals:

public slots:
    void finished(int);
};

#endif // JOBSCHEDULER_H
