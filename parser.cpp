#include "parser.h"

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

            QJsonParseError status;

            QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &status);

            file.close();

            if(QJsonParseError::NoError == status.error)
            {
                if(filename.contains("category"))
                {
                    dealWithCategories(doc);
                }
                else if(filename.contains("product"))
                {
                    dealWithProducts(doc);
                }
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

            qDebug() << id << name << path.join(',');
        }
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

                qDebug() << upc << url << name << start_date << regular_price << sale_price << availability << reviews << path;
            }
        }
    }
}
