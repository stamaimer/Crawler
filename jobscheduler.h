#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "walmart.h"
#include "inserter.h"

#include "menu.h"
#include "merchandise.h"

#include <QMutex>

#include <QString>

#include <QVector>
#include <QStringList>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QDebug>
#include <QElapsedTimer>

#define AMOUNT_OF_THREADS 200

class Requester;
class QNetworkReply;

class JobScheduler
{
    QElapsedTimer timer;

    QVector<Menu> menus;

    QVector<QString> completed;

    Requester* requesters[AMOUNT_OF_THREADS];

    bool getJsonDoc(QNetworkReply*, Walmart*, QJsonDocument*);

public:
    QMutex mutex;

    QVector<Walmart*> walmarts;

    QVector<Merchandise> merchandises;

    Inserter* inserter;

    JobScheduler();

    bool getMenus(QNetworkReply*, Walmart*);
    bool getMerchandise(QNetworkReply*, Walmart*);
};

#endif // JOBSCHEDULER_H
