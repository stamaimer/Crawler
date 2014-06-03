#include "merchandise.h"

Merchandise::Merchandise(QString upc,
                         QString url,
                         QString src,
                         QString name,
                         QString date,
                         double msrp,
                         double price,
                         bool stock,
                         int reviews,
                         QStringList path)
{
    this->upc     = upc;
    this->url     = url;
    this->src     = src;
    this->name    = name;
    this->date    = date;
    this->path    = path;
    this->msrp    = msrp;
    this->price   = price;
    this->stock   = stock;
    this->reviews = reviews;
}

QString Merchandise::getUPC()
{
    return upc;
}

QString Merchandise::getURL()
{
    return url;
}

QString Merchandise::getSRC()
{
    return src;
}

QString Merchandise::getName()
{
    return name.toHtmlEscaped().remove("\\");
}

QString Merchandise::getDate()
{
    return date;
}

QString Merchandise::getPath()
{
    return path.join(">>");
}

double Merchandise::getMSRP()
{
    return msrp;
}

double Merchandise::getPrice()
{
    return price;
}

bool Merchandise::getStock()
{
    return stock;
}

int Merchandise::getReviews()
{
    return reviews;
}
