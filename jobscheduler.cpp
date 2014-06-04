#include "jobscheduler.h"
#include "requester.h"

JobScheduler::JobScheduler(QObject *parent) : QObject(parent)
{
    timer.start();

    //stamaimer@gmail.com
    apikeys.append("4pj6ws82bq85vafs2369bdeu");
    apikeys.append("u72crv2skdzyp5nqy7ennku8");
    apikeys.append("6xahv45r4smxyrw2wx96n66z");
    //2518930466@qq.com
    apikeys.append("3qpv23xtm5wy9ypwcf5r9tpv");
    apikeys.append("9e8eyabbk2pcztteqcsy4vna");
    apikeys.append("m963nacae9kspgjbw3nj3zhc");
    //stamaimer@live.com
    apikeys.append("xtqc3kj5aevzeha8ak5tc5yj");
    apikeys.append("s4kyeuwjhh3r4zzehczfx7ms");
    apikeys.append("cphh93x6cjb954293axvtnss");
    //angelsister@163.com
    apikeys.append("yum5pts2rnyvtr4mc93u5gab");
    apikeys.append("6t2t5uwcm97a7jcab83wy48a");
    apikeys.append("66w2q8kcdvqg44cm627j3q97");
    //spider_bestbuy_01@163.com
    apikeys.append("aup37y5y3kry5zxpmqfcuj6g");
    apikeys.append("g2xr6qrt2u48356ue56mnzdt");
    apikeys.append("ae7fj8n2nx9agb8sm5mv2y32");
    //spider_bestbuy_02@163.com
    apikeys.append("tdf9j5z5zdbp2xvp5cq68ewh");
    apikeys.append("7hyvbj7a438m8geb832y2vc7");
    apikeys.append("xzuw3hay7vvzuzjheub55swn");
    //spider_bestbuy_03@163.com
    apikeys.append("xhb7na6kpz62uxydm9mn7bjp");
    apikeys.append("bq2jdr5p3hw4nzntykq546fk");
    apikeys.append("mu2tv8zgmvn3ezwy7xpkg3za");
    //spider_bestbuy_04@163.com






//    inserter = new Inserter();

    //添加目錄種子
    //bestbuys.append(new BestBuy("http://api.remix.bestbuy.com/v1/categories?show=id,name,path.id&format=json&pageSize=100&page=1&apiKey=%1", 0));

    //添加商品種子
    bestbuys.append(new BestBuy("http://api.remix.bestbuy.com/v1/products(sku=*)?show=upc,sku,url,name,source,startDate,salePrice,regularPrice,onlineAvailability,customerReviewCount,categoryPath.id&format=json&pageSize=100&page=1&apiKey=%1", 0));

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        requesters[i] = new Requester(i, apikeys[i % 21], this);

        requesters[i]->start();

        tids.append(i);
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
            qDebug() << "PARSE ERROR" << bestbuy->request_url << parse_status.error << parse_status.errorString();
        }
    }
    else
    {
        qDebug() << "NETWORK ERROR" << bestbuy->request_url << reply->error() << reply->errorString();
    }

    if(bestbuy->request_count < MAX_REQUEST_COUNT)
    {
        bestbuys.append(bestbuy);
    }
    else
    {
        delete bestbuy;
    }

    return false;
}

bool JobScheduler::getMenus(QNetworkReply* reply, BestBuy* bestbuy)
{
    QJsonDocument* doc = new QJsonDocument();

    if(getJsonDoc(reply, bestbuy, doc))
    {
        if(doc->isObject())
        {
            QJsonObject result = doc->object();

            if(bestbuy->request_url.contains("page=1&"))//what about page=10,page=100,page=1000
            {
                int totalPages = result["totalPages"].toInt();

                QString pattern = "http://api.remix.bestbuy.com/v1/categories?show=id,name,path.id&format=json&pageSize=100&page=%1&apiKey=";

                for(int i = 2; i <= totalPages; ++i)
                {
                    if(!completed.contains(pattern.arg(i).append("%1")))
                    {
                        bestbuys.append(new BestBuy(pattern.arg(i).append("%1"), 0));
                    }
                }
            }

            //数据处理

            QJsonArray categories = result["categories"].toArray();

            QJsonObject category;

            for(int i = 0; i < categories.size(); ++i)
            {
                category = categories[i].toObject();

                QString id   = category["id"].toString();
                QString name = category["name"].toString();

                QJsonArray tmp = category["path"].toArray();

                QStringList path;

                for(int j = 0; j < tmp.size(); ++j)
                {
                    path << tmp[j].toObject()["id"].toString();
                }

                qDebug() << id << name << path;

//                menus.append(Menu(id, name, path));
            }

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

            if(bestbuy->request_url.contains("page=1&"))//what about page=10,page=100,page=1000
            {
                int totalPages = result["totalPages"].toInt();

                QString pattern = "http://api.remix.bestbuy.com/v1/products(sku=*)?show=upc,sku,url,name,source,startDate,salePrice,regularPrice,onlineAvailability,customerReviewCount,categoryPath.id&format=json&pageSize=100&page=%1&apiKey=";

                for(int i = 2; i < totalPages; ++i)
                {
                    if(!completed.contains(pattern.arg(i).append("%1")))
                    {
                        bestbuys.append(new BestBuy(pattern.arg(i).append("%1"), 0));
                    }
                }
            }

            //数据处理

            QJsonArray products = result["products"].toArray();

            QJsonObject product;

            for(int i = 0; i < products.size(); ++i)
            {
                product = products[i].toObject();

                QString upc     = product["upc"].toString();
                QString url     = product["url"].toString();
                QString src     = product["source"].toString();

                QString name    = product["name"].toString();
                QString date    = product["startDate"].toString();

                double msrp    = product["regularPrice"].toDouble();
                double price   = product["salePrice"].toDouble();

                bool stock   = product["onlineAvailability"].toBool();

                int reviews = product["customerReviewCount"].toInt();

                QJsonArray tmp = product["categoryPath"].toArray();

                QStringList path;

                for(int j = 0; j < tmp.size(); ++j)
                {
                    path << tmp[j].toObject()["id"].toString();
                }

                qDebug() << upc /*<< src << name << date << msrp << price << stock << reviews << path*/;

//                merchandises.append(Merchandise(upc, url, src, name, date, msrp, price, stock, reviews, path));
            }

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
//        QSqlDatabase::database().transaction();

//        inserter->insert(merchandises);

//        QSqlDatabase::database().commit();

        qDebug() << "THREAD" << tid + AMOUNT_OF_THREADS << "FINISHED" << "TOTAL:" << count;

        qDebug() << "EXIT...";

        qDebug() << (double)(timer.elapsed() / 60000) << "min elapsed";

//        delete inserter;

        exit(0);
    }
    else
    {
        qDebug() << "THREAD" << tid + AMOUNT_OF_THREADS << "FINISHED" << "TOTAL:" << count;

        qDebug() << (double)(timer.elapsed() / 60000) << "min elapsed";

        mutex.unlock();
    }
}
