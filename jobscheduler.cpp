#include "jobscheduler.h"
#include "requester.h"

JobScheduler::JobScheduler()
{
    menus.append(Menu("bdc5bab64b1f84fa7b6b301c26fdf439af96a3a0", "Electronics", "3944", QStringList()));
    menus.append(Menu("d12bf578c2846aee54b8bafc892baabd1d37c97e", "Video Games", "2636", QStringList()));
    menus.append(Menu("7815b575f44ce887a8a23d06704cf765bfb0502c", "Office", "546952", QStringList()));
    menus.append(Menu("09440c9a919015c0343ae111e4726432aacbb30f", "Toys", "4171", QStringList()));

    for(int i = 0; i < menus.size(); ++i)
    {
        walmarts.append(new Walmart(menus[i].getName(), QString("http://api.mobile.walmart.com/taxonomy/departments/%1").arg(menus[i].getId())));
    }

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        requesters[i] = new Requester(this);

        requesters[i]->start(i + AMOUNT_OF_THREADS);
    }
}

bool JobScheduler::getJsonDoc(QNetworkReply* reply, Walmart* walmart, QJsonDocument* doc)
{
    QByteArray      reply_body;
    QJsonParseError parse_result;

    if(reply->error() == QNetworkReply::NoError)
    {

        reply_body = reply->readAll();

        *doc = QJsonDocument::fromJson(reply_body, &parse_result);

        if(parse_result.error == QJsonParseError::NoError)
        {
            completed.append(walmart->request_url);

            return true;
        }
        else
        {
            qDebug() << walmart->name << parse_result.error << parse_result.errorString();

            qDebug() << reply_body;

            return false;
        }
    }
    else
    {
        qDebug() << walmart->name << reply->error() << reply->errorString();

        walmarts.append(walmart);

        return false;
    }
}

void JobScheduler::getMenus(QNetworkReply* reply, Walmart* walmart)
{
    QJsonDocument* doc = new QJsonDocument();

    if(getJsonDoc(reply, walmart, doc))
    {
        if(doc->isObject())
        {
            QJsonArray menus = doc->object()["children"].toArray();

            QJsonObject item;

            for(int i = 0; i < menus.size(); ++i)
            {
                item =  menus[i].toObject();

                QString id = item["id"].toString();

                QString name = item["name"].toString();

                QString category = item["category"].toString();

                QJsonArray tmp =  item["parentCategories"].toArray();

                QStringList parent_categories;

                for(int i = 0; i < tmp.size(); ++i)
                {
                    parent_categories << tmp[i].toString();
                }

                this->menus.append(Menu(id, name, category, parent_categories));

                if(item.contains("browseToken"))
                {
                    QString browse_token = item["browseToken"].toString();

                    QString request_url = QString("http://mobile.walmart.com/m/j?service=Browse&method=browseByToken&p1=%1&p2=All&p3=RELEVANCE&p4=0&p5=20").arg(browse_token);

                    if(!completed.contains(request_url))
                    {
                        walmarts.append(new Walmart(name, request_url));
                    }
                }
                else
                {
                    QString id   = item["id"].toString();

                    QString request_url = "http://api.mobile.walmart.com/taxonomy/departments/" + id;

                    if(!completed.contains(request_url))
                    {
                        walmarts.append(new Walmart(name, request_url));
                    }
                }
            }
        }
        else
        {
            qDebug() << "47 structure of reply invalidation";
        }
    }
}

void JobScheduler::getMerchandise(QNetworkReply* reply, Walmart* walmart)
{
    QJsonDocument* doc = new QJsonDocument();

    if(getJsonDoc(reply, walmart, doc))
    {
        if(doc->isObject())
        {
            QJsonArray items = doc->object()["item"].toArray();

            QJsonObject item;

            for(int i = 0; i < items.size(); ++i)
            {
                 item =  items[i].toObject();

                 qDebug() << item["name"].toString() << item["cRRNumReviews"].toString();
            }
        }
        else
        {
            qDebug() << "47 structure of reply invalidation";
        }
    }
}
