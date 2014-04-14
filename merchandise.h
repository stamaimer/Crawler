#ifndef MERCHANDISE_H
#define MERCHANDISE_H

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

public:
    Merchandise(){}

    Merchandise(QString id,
                QString url,
                QString name,
                QString msrp,
                QString price,
                QString stock,
                QString reviews)
    {
        this->id      = id;
        this->url     = url;
        this->name    = name;
        this->msrp    = msrp;
        this->price   = price;
        this->stock   = stock;
        this->reviews = reviews;
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
            QStringList strs = msrp.split('$');

            QString str = strs[1];

            if(str.contains(','))
            {
                str.remove(str.indexOf(','));
            }

            return str.toDouble();
        }
    }

    double getPrice()
    {
        QStringList strs = price.split('$');

        QString str = strs[1];

        if(str.contains(','))
        {
            str.remove(str.indexOf(','));
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
};

#endif // MERCHANDISE_H
