#include "jobscheduler.h"
#include "requester.h"

JobScheduler::JobScheduler()
{
    timer.start();

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
        requesters[i] = new Requester(i, this);

        requesters[i]->start();
    }
}

bool JobScheduler::getJsonDoc(QNetworkReply* reply, Walmart* walmart, QJsonDocument* doc)
{
    QByteArray      reply_body;
    QJsonParseError parse_status;

    if(reply->error() == QNetworkReply::NoError)
    {

        reply_body = reply->readAll();

        *doc = QJsonDocument::fromJson(reply_body, &parse_status);

        if(parse_status.error == QJsonParseError::NoError)
        {
            completed.append(walmart->request_url);

            return true;
        }
        else
        {
            qDebug() << walmart->name << parse_status.error << parse_status.errorString();

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

            QJsonObject menu;

            for(int i = 0; i < menus.size(); ++i)
            {
                menu =  menus[i].toObject();

                QString id       = menu["id"].toString();

                QString name     = menu["name"].toString();

                QString category = menu["category"].toString();

                QJsonArray tmp   =  menu["parentCategories"].toArray();

                QStringList parent_categories;

                for(int i = 0; i < tmp.size(); ++i)
                {
                    parent_categories << tmp[i].toString();
                }

                qDebug() << id                << '\t'
                         << name              << '\t'
                         << category          << '\t'
                         << parent_categories << '\t';

                this->menus.append(Menu(id, name, category, parent_categories));

                if(menu.contains("browseToken"))
                {
                    QString browse_token = menu["browseToken"].toString();

                    QString request_url  = QString("http://mobile.walmart.com/m/j?service=Browse&method=browseByToken&p1=%1&p2=All&p3=RELEVANCE&p4=0&p5=20").arg(browse_token);

                    if(!completed.contains(request_url))
                    {
                        walmarts.append(new Walmart(name, request_url));
                    }
                }
                else
                {
                    QString id          = menu["id"].toString();

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
            qDebug() << "47 structure of doc invalidation";
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

                 QString id      = item["iD"].toString();
                 QString name    = item["name"].toString();
                 QString msrp    = item["pricingInformation"].toObject()["listPrice"].toString();
                 QString price   = item["price"].toString();
                 QString stock   = item["addableToCart"].toString();
                 QString reviews = item["cRRNumReviews"].toString();

                 qDebug() << id      << '\t'
                          << name    << '\t'
                          << msrp    << '\t'
                          << price   << '\t'
                          << stock   << '\t'
                          << reviews << '\t';

                 this->merchandises.append(Merchandise(id, name, msrp, price, stock, reviews));
            }
        }
        else
        {
            qDebug() << "47 structure of doc invalidation";
        }
    }
}
