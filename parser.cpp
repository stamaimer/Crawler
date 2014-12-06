#include "parser.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"

using namespace rapidjson;

Parser::Parser(int tid, Scheduler* scheduler)
{
    this->tid 		= tid;
    this->scheduler = scheduler;
}

void Parser::run()
{
    connect(this, SIGNAL(finished(int)), scheduler, SLOT(finished(int)), Qt::DirectConnection);

    while(true)
    {
        scheduler->mutex.lock();

        if(scheduler->files.size() != 0)
        {
            QString filename = scheduler->files.takeFirst();

            scheduler->mutex.unlock();

            QFile file(filename);

            if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                qDebug() << "file open faild!!!";
            }

            QByteArray json = file.readAll();

            QJsonParseError status;

            QJsonDocument doc = QJsonDocument::fromJson(json, &status);

            file.close();

            if(QJsonParseError::NoError == status.error)
            {
                if(filename.contains("categories"))
                {
                    dealWithCategories(doc);
                }
                else if(filename.contains("products"))
                {
                    dealWithProducts(doc);
                }
            }
            else if(QJsonParseError::DocumentTooLarge == status.error)
            {
                dealWithLargeDoc(json);
            }
            else
            {
                qDebug() << status.error << status.errorString();
            }
        }
        else
        {
            scheduler->mutex.unlock();

            break;
        }
    }

    emit finished(tid);
}

void Parser::dealWithCategories(QJsonDocument doc)
{
    if(doc.isArray())
    {
        QJsonArray categories = doc.array();

        QJsonObject category;

        for(int i = 0; i < categories.size(); ++i)
        {
            category = categories[i].toObject();

            QString id = category["id"].toString();
            QString name = category["name"].toString();

            QJsonArray tmp = category["path"].toArray();

            QStringList path;

            for(int j = 0; j < tmp.size(); ++j)
            {
                path << tmp[j].toObject()["id"].toString();
            }

//            qDebug() << id << name << path.join(',');

            this->categories.append(Category(id, name, path));
        }

        scheduler->mutex.lock();

        scheduler->inserter->insert(this->categories);

        this->categories.clear();

        scheduler->mutex.unlock();
    }
}

void Parser::dealWithProducts(QJsonDocument doc)
{
    if(doc.isArray())
    {
        QJsonArray products = doc.array();

        QJsonObject product;

        for(int i = 0; i < products.size(); ++i)
        {
            product = products[i].toObject();

            if(product["active"].toBool() && product["source"].toString().contains("bestbuy"))
            {
                int sku = product["sku"].toInt();

                QString upc = product["upc"].toString();
                QString url = product["url"].toString();

                QString name = product["name"].toString();
                QString start_date = product["startDate"].toString();

                double regular_price = product["regularPrice"].toDouble();
                double sale_price = product["salePrice"].toDouble();

                bool availability = product["onlineAvailability"].toBool();

                int reviews = product["customerReviewCount"].toInt();

                QJsonArray tmp = product["categoryPath"].toArray();

                QStringList path;

                for(int j = 0; j < tmp.size(); ++j)
                {
                    path << tmp[j].toObject()["id"].toString();
                }

//                qDebug() << upc << url << name << start_date << regular_price << sale_price << availability << reviews << path;

                this->products.append(Product(sku, upc, url, name, start_date, regular_price, sale_price, availability, reviews, path));
            }
        }

        scheduler->mutex.lock();

        scheduler->inserter->insert(this->products);

        this->products.clear();

        scheduler->mutex.unlock();
    }
}

/*because some file are too large to construct json object in Qt,
 *we choose to parse these file line by line. but we remain the
 *function unimplemented until ...
 */

void Parser::dealWithLargeDoc(QByteArray json)
{
    const char* data = json.constData();

    Document doc;

    if(doc.Parse(data).HasParseError())
    {
        qDebug() << "big file parse error!!!";
    }
    else
    {
        if(doc.IsArray())
        {
            for(SizeType i = 0; i < products.Size(); ++i)
            {
                Document products;

                products.SetArray();

                products.PushBack(doc[i], products.GetAllocator());

                StringBuffer strbuf;

                Writer<StringBuffer> writer(strbuf);

                products.Accept(writer);

                QJsonParseError status;

                QJsonDocument doc = QJsonDocument::fromJson(QByteArray(strbuf.GetString()), &status);

                if(QJsonParseError::NoError == status.error)
                {
                    dealWithProducts(doc);
                }
                else
                {
                    qDebug() << status.error << status.errorString();
                }
            }
        }
        else
        {
            qDebug() << "file structure error!!!";
        }
    }
}























