#ifndef MERCHANDISE_H
#define MERCHANDISE_H

#include <QDebug>
#include <QRegExp>
#include <QString>
#include <QStringList>

class Merchandise
{
    QString id;
    QString url;
    QString name;
    QString msrp;
    QString price;
    QString stock;
    QString reviews;
    QString parent;

public:
    Merchandise(){}

    Merchandise(QString id,
                QString url,
                QString name,
                QString msrp,
                QString price,
                QString stock,
                QString reviews,
                QString parent)
    {
        this->id      = id;
        this->url     = url;
        this->name    = name;
        this->msrp    = msrp;
        this->price   = price;
        this->stock   = stock;
        this->reviews = reviews;
        this->parent  = parent;
    }

    int getId()
    {
        return id.toInt();
    }

    QString getURL()
    {
        return url;
    }

    QString getName()
    {
        return name.toHtmlEscaped();
    }

    double getMSRP()
    {
        if(msrp.isEmpty())
        {
            return getPrice();
        }
        else
        {
//            qDebug() << msrp;

            QString str = msrp.split('$')[1];

            if(str.contains(','))
            {
                str.remove(',');
            }

            return str.toDouble();
        }
    }

    double getPrice()
    {
//        qDebug() << price;

        QString str = price.split('$')[1];

        if(str.contains(','))
        {
            str.remove(',');
        }

        return str.toDouble();
    }

    int getStock()
    {
        return "true" == stock ? 1 : 0;
    }

    int getReviews()
    {
        return reviews.toInt();
    }

    QString getParent()
    {
        return parent;
    }
};

#endif // MERCHANDISE_H
