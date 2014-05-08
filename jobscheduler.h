#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "walmart.h"

#include "inserter.h"

#include"utils.h"

#include "menu.h"
#include "merchandise.h"

#include <QFile>

#include <QMutex>

#include <QObject>

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

class JobScheduler : public QObject
{
    Q_OBJECT

    QVector<Menu> menus;

    QVector<QString> completed;

    Requester* requesters[AMOUNT_OF_THREADS];

    bool getJsonDoc(QNetworkReply*, Walmart*, QJsonDocument*);

public:
    QMutex mutex;

    QElapsedTimer timer;

    QVector<Walmart*> walmarts;

    QVector<Merchandise> merchandises;

    Inserter* inserter;

    Utils utils;

    JobScheduler();

    bool getMenus(QNetworkReply*, Walmart*);
    bool getMerchandise(QNetworkReply*, Walmart*);

private slots:
    void finished(int);
};

#endif // JOBSCHEDULER_H
