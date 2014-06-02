#include "jobscheduler.h"
#include "requester.h"

JobScheduler::JobScheduler(QObject *parent) : QObject(parent)
{
    timer.start();

    apikeys.append("4pj6ws82bq85vafs2369bdeu");
    apikeys.append("u72crv2skdzyp5nqy7ennku8");
    apikeys.append("6xahv45r4smxyrw2wx96n66z");
//    apikeys.append("j5mtfebugzwtz28rafxv8kva");
//    apikeys.append("drmxqjgmdqc9ayrjezytz5qn");
//    apikeys.append("a2uh8n3ehabw7b5qumsswcey");
//    apikeys.append("2svry3naj58a89uzbhrwgusx");
//    apikeys.append("k5cqemwxdmfmkhqwtd3vtx6h");
//    apikeys.append("kwf7gz2rbkyzvtb5mhm45wru");
//    apikeys.append("9h3tjmnkgu8a7ephq2xmbs8s");
    apikeys.append("3qpv23xtm5wy9ypwcf5r9tpv");
    apikeys.append("9e8eyabbk2pcztteqcsy4vna");
    apikeys.append("m963nacae9kspgjbw3nj3zhc");
//    apikeys.append("2qf4mnss3pzev3mu9zxyjny9");
//    apikeys.append("bsk64fdk85mns4yfhx88sy8f");
//    apikeys.append("yrbyfarkhy2pa7qvfebe5e5e");
//    apikeys.append("ryphsm73kjm6acv7xxc3a53h");
//    apikeys.append("5dach4tzku6hx3gcmsndx6zt");
//    apikeys.append("af6t883wgqdf67n2pue6vyub");
//    apikeys.append("d2vrhup2ag5v5xnux2umgxvq");

//    inserter = new Inserter();

    //添加目錄種子
    //bestbuys.append(new BestBuy("http://api.remix.bestbuy.com/v1/categories?show=id,name,path.id&format=json&pageSize=100&page=1&apiKey=%1", 0));

    //添加商品種子
    bestbuys.append(new BestBuy("http://api.remix.bestbuy.com/v1/products(sku=*)?show=upc,sku,url,name,source,startDate,salePrice,regularPrice,onlineAvailability,customerReviewCount,categoryPath.id&format=json&pageSize=100&page=1&apiKey=%1", 0));

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        requesters[i] = new Requester(i, apikeys[i % 6], this);

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

                //menus.append(Menu(id, name, path));
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

                QString src     = product["source"].toString();
                QString upc     = product["upc"].toString();
                QString url     = product["url"].toString();
                QString name    = product["name"].toString();
                QString date    = product["startDate"].toString();
                int msrp    = product["regularPrice"].toInt();
                int price   = product["salePrice"].toInt();
                bool stock   = product["onlineAvailability"].toBool();
                int reviews = product["customerReviewCount"].toInt();

                QJsonArray tmp = product["categoryPath"].toArray();

                QStringList path;

                for(int j = 0; j < tmp.size(); ++j)
                {
                    path << tmp[j].toObject()["id"].toString();
                }

                qDebug() << upc << name << date << msrp << price << stock << reviews << path;

                if(src.contains("bestbuy"))
                {
                    //merchandises.append(Merchandise(upc, sku, url, name, date, msrp, price, stock, reviews, path));
                }
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

        mutex.unlock();
    }
}
