#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "walmart.h"

#include "menu.h"
#include "merchandise.h"

#include <QObject>

#include <QMutex>

#include <QNetworkReply>

#include <QDebug>
#include <QElapsedTimer>

#include <QString>

#include <QVector>
#include <QStringList>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#define AMOUNT_OF_THREADS 100

class Requester;

class JobScheduler : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

    Requester* requesters[AMOUNT_OF_THREADS];

    bool getJsonDoc(QNetworkReply*, Walmart*, QJsonDocument*);

public:
    QMutex mutex;

    QVector<Walmart*> walmarts;

    QVector<Menu> menus;

    QVector<Merchandise> merchandises;

    QVector<QString> completed;

    explicit JobScheduler(QObject *parent = 0);

signals:

public slots:
    void getMenus(QNetworkReply*, Walmart*);
    void getMerchandise(QNetworkReply*, Walmart*);
};

#endif // JOBSCHEDULER_H
