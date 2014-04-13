#ifndef MERCHANDISE_H
#define MERCHANDISE_H

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

    QString getId()
    {
        return id;
    }

    QString getName()
    {
        return name;
    }

    QString getMSRP()
    {
        if(msrp.isEmpty())
        {
            getPrice();
        }
        else
        {
            return msrp;
        }
    }

    QString getPrice()
    {
        return price;
    }

    QString getStock()
    {
        return stock;
    }

    QString getReviews()
    {
        return reviews;
    }
};

#endif // MERCHANDISE_H
