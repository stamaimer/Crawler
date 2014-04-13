#ifndef MERCHANDISE_H
#define MERCHANDISE_H

#include <QRegExp>
#include <QString>

class Merchandise
{
    QString id;
    QString name;
    QString msrp;
    QString price;
    QString stock;
    QString reviews;

public:
    Merchandise(){}

    Merchandise(QString id,
                QString name,
                QString msrp,
                QString price,
                QString stock,
                QString reviews)
    {
        this->id      = id;
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
            return msrp.replace(QRegExp("$"), "").toDouble();
        }
    }

    double getPrice()
    {
        return price.replace(QRegExp("$"), "").toDouble();
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
