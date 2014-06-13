#include "product.h"

Product::Product(QString upc,
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

QString Product::getUPC()
{
    return upc;
}

QString Product::getURL()
{
    return url;
}

QString Product::getSRC()
{
    return src;
}

QString Product::getName()
{
    return name.toHtmlEscaped().remove("\\");
}

QString Product::getDate()
{
    return date;
}

QString Product::getPath()
{
    return path.join(">>");
}

double Product::getMSRP()
{
    return msrp;
}

double Product::getPrice()
{
    return price;
}

int Product::getStock()
{
    return true == stock ? 1 : 0;
}

int Product::getReviews()
{
    return reviews;
}
