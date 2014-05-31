#include "jobscheduler.h"
#include "requester.h"

JobScheduler::JobScheduler(QObject *parent) : QObject(parent)
{
    timer.start();

    qDebug() << "计时器启动";

//    inserter = new Inserter();

    //添加目錄種子
    bestbuys.append(new BestBuy("http://api.remix.bestbuy.com/v1/categories?format=json&apiKey=4pj6ws82bq85vafs2369bdeu", 0));

    //添加商品種子
    bestbuys.append(new BestBuy("http://api.remix.bestbuy.com/v1/products(sku=*)?format=json&apiKey=4pj6ws82bq85vafs2369bdeu", 0));

    qDebug() << "添加种子链接";

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        requesters[i] = new Requester(i, this);

        requesters[i]->start();

        tids.append(i);
    }

    qDebug() << "启动多个线程";
}

bool JobScheduler::getJsonDoc(QNetworkReply* reply, BestBuy* bestbuy, QJsonDocument* doc)
{
    QByteArray reply_body;

    QJsonParseError parse_status;

    bestbuy->request_count++;//请求次数递增

    if(reply->error() == QNetworkReply::NoError)
    {
        reply_body = reply->readAll();

        qDebug() << "获取响应信息";

        *doc = QJsonDocument::fromJson(QString(reply_body).toUtf8(), &parse_status);

        qDebug() << "使用响应信息构造JsonDocument";

        if(parse_status.error == QJsonParseError::NoError)
        {
            qDebug() << "成功构造JsonDocument";

            completed.append(bestbuy->request_url);

            return true;
        }
        else
        {
            qDebug() << "PARSE ERROR" << bestbuy->request_url << parse_status.error << parse_status.errorString();
        }
    }
    else
    {
        qDebug() << "NETWORK ERROR" << bestbuy->request_url << reply->error() << reply->errorString();
    }

    if(bestbuy->request_count < MAX_REQUEST_COUNT)
    {
        qDebug() << "重新添加错误条目";

        bestbuys.append(bestbuy);
    }
    else
    {
        qDebug() << "条目错误词数过多,舍弃";

        delete bestbuy;
    }

    return false;
}

bool JobScheduler::getMenus(QNetworkReply* reply, BestBuy* bestbuy)
{
    QJsonDocument* doc = new QJsonDocument();

    qDebug() << "新建JsonDocument对象";

    if(getJsonDoc(reply, bestbuy, doc))
    {
        if(doc->isObject())
        {
            QJsonObject result = doc->object();

            QJsonArray categories = result["categories"].toArray();

            if(!bestbuy->request_url.contains("page=1"))//what about page=10,page=100,page=1000
            {
                int totalPages = result["totalPages"].toInt();

                QString pattern = "http://api.remix.bestbuy.com/v1/categories?format=json&pageSize=100&page=%1&apiKey=4pj6ws82bq85vafs2369bdeu";

                for(int i = 2; i <= totalPages; ++i)
                {
                    bestbuys.append(new BestBuy(pattern.arg(i), 0));
                }
            }

            //数据处理
            //qDebug() << categories;
            qDebug() << "IN GET MENUS" << bestbuy->request_url;

            delete doc;
            delete bestbuy;
            reply->deleteLater();
            return true;
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
        if(doc->isObject())
        {
            QJsonObject result = doc->object();

            QJsonArray products = result["products"].toArray();

            if(!bestbuy->request_url.contains("page=1"))//what about page=10,page=100,page=1000
            {
                int totalPages = result["totalPages"].toInt();

                QString pattern = "http://api.remix.bestbuy.com/v1/products(sku=*)?format=json&pageSize=100&page=%1&apiKey=4pj6ws82bq85vafs2369bdeu";

                for(int i = 2; i < totalPages; ++i)
                {
                    bestbuys.append(new BestBuy(pattern.arg(i), 0));
                }
            }

            //数据处理
            //qDebug() << products;
            qDebug() << "IN GET MERCH" << bestbuy->request_url;

            delete doc;
            delete bestbuy;
            reply->deleteLater();
            return true;
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
