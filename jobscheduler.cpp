#include "jobscheduler.h"

JobScheduler::JobScheduler(QObject *parent) : QObject(parent)
{
    timer.start();

    //menus.append();

    for(int i = 0; i < menus.size(); ++i)
    {
        //bestbuys.append();
    }

    inserter = new Inserter();

    for(int i = 0; i < menus.size(); ++i)
    {
        inserter->insert(menus[i]);
    }

    //

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        //
    }
}

bool JobScheduler::getJsonDoc(QNetworkReply* reply, BestBuy* bestbuy, QJsonDocument* doc)
{
    QByteArray reply_body;

    QJsonParseError parse_status;

    bestbuy->request_count++;//请求次数递增

    if(reply->error() == QNetworkReply::NoError)
    {
        reply_body = reply->readAll();

        *doc = QJsonDocument::fromJson(QString(reply_body).toUtf8(), &parse_status);

        if(parse_status.error == QJsonParseError::NoError)
        {
            completed.append(bestbuy->request_url);

            return true;
        }
        else
        {
            qDebug() << bestbuy->name << parse_status.error << parse_status.errorString();
        }
    }
    else
    {
        qDebug() << bestbuy->name << reply->error() << reply->errorString();
    }

    if(bestbuy->request_count < MAX_REQUEST_COUNT)
    {
        bestbuys.append(bestbuy);
    }

    return false;
}

bool JobScheduler::getMenus(QNetworkReply* reply, BestBuy* bestbuy)
{
    QJsonDocument* doc = new QJsonDocument();

    if(getJsonDoc(reply, bestbuy, doc))
    {
        if()
        {
            delete doc;
            delete bestbuy;
            reply->deleteLater();
            return true;
        }
        else
        {
            qDebug() << "47 structure of doc invalidation";

            bestbuys.append(bestbuy);
        }
    }

    delete doc;
    reply->deleteLater();
    return false;
}

bool JobScheduler::getMerchandises(QNetworkReply* reply, BestBuy* bestbuy)
{
    QJsonDocument* doc = new QJsonDocument();

    if(getJsonDoc(reply, bestbuy, doc))
    {
        if()
        {
            delete doc;
            delete bestbuy;
            reply->deleteLater();
            return true;
        }
        else
        {
            qDebug() << "47 structure of doc invalidation";

            bestbuys.append(bestbuy);
        }
    }

    delete doc;
    reply->deleteLater();
    return false;
}

void JobScheduler::finished(int tid)
{
    static int count = 0;

    requesters[tid]->exit();

    mutex.lock();

    tids.remove(tids.indexOf(tid));

    qDebug() << tids;

    if(AMOUNT_OF_THREADS == ++count)
    {
        QSqlDatabase::database().transaction();

        inserter->insert(merchandises);

        QSqlDatabase::database().commit();

        qDebug() << "THREAD" << tid + AMOUNT_OF_THREADS << "FINISHED" << "TOTAL:" << count;

        qDebug() << "EXIT...";

        qDebug() << (double)(timer.elapsed() / 60000) << "min elapsed";

        delete inserter;

        exit(0);
    }
    else
    {
        qDebug() << "THREAD" << tid + AMOUNT_OF_THREADS << "FINISHED" << "TOTAL:" << count;

        mutex.unlock();
    }
}
