#ifndef MERCHANDISE_H
#define MERCHANDISE_H

#include <QString>

class Merchandise
{
    QString merchandise_id;
    QString merchandise_name;
    QString merchandise_msrp;
    QString merchandise_price;
    QString merchandise_stock;
    QString merchandise_reviews;

public:
    Merchandise(){}

    Merchandise(QString merchandise_id,
                QString merchandise_name,
                QString merchandise_msrp,
                QString merchandise_price,
                QString merchandise_stock,
                QString merchandise_reviews)
    {
        this->merchandise_id      = merchandise_id;
        this->merchandise_name    = merchandise_name;
        this->merchandise_msrp    = merchandise_msrp;
        this->merchandise_price   = merchandise_price;
        this->merchandise_stock   = merchandise_stock;
        this->merchandise_reviews = merchandise_reviews;
    }

    QString getMerchandiseId()
    {
        return merchandise_id;
    }

    QString getMerchandiseName()
    {
        return merchandise_name;
    }

    QString getMerchandiseMSRP()
    {
        if(merchandise_msrp.isEmpty())
        {
            getMerchandisePrice();
        }
        else
        {
            return merchandise_msrp;
        }
    }

    QString getMerchandisePrice()
    {
        return merchandise_price;
    }

    QString getMerchandiseStock()
    {
        return merchandise_stock;
    }

    QString getMerchandiseReviews()
    {
        return merchandise_reviews;
    }
};

#endif // MERCHANDISE_H
