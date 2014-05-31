#include "merchandise.h"

Merchandise::Merchandise(QString upc,
                         QString sku,
                         QString url,
                         QString name,
                         QString date,
                         QString msrp,
                         QString price,
                         QString stock,
                         QString reviews,
                         QStringList path)
{
    this->upc     = upc;
    this->sku     = sku;
    this->url     = url;
    this->name    = name;
    this->date    = date;
    this->msrp    = msrp;
    this->path    = path;
    this->price   = price;
    this->stock   = stock;
    this->reviews = reviews;
}

QString Merchandise::getUPC()
{
    return upc;
}

QString Merchandise::getSKU()
{
    return sku;
}

QString Merchandise::getURL()
{
    return url;
}

QString Merchandise::getName()
{
    return name;
}

QString Merchandise::getDate()
{
    return date;
}

QString Merchandise::getMSRP()
{
    return msrp;
}

QString Merchandise::getPath()
{
    return path.join(">>");
}

QString Merchandise::getPrice()
{
    return price;
}

QString Merchandise::getStock()
{
    return stock;
}

QString Merchandise::getReviews()
{
    return reviews;
}
