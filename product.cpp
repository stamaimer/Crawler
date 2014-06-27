#include "product.h"

Product::Product(int sku,
                 QString upc,
                 QString url,
                 QString name,
                 QString date,
                 double msrp,
                 double price,
                 bool stock,
                 int reviews,
                 QStringList path)
{
    this->sku	  = sku;
    this->upc     = upc;
    this->url     = url;
    this->name    = name;
    this->date    = date;
    this->path    = path;
    this->msrp    = msrp;
    this->price   = price;
    this->stock   = stock;
    this->reviews = reviews;
}

int Product::getSKU()
{
    return sku;
}

QString Product::getUPC()
{
    return upc;
}

QString Product::getURL()
{
    return url;
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
    return path.join(",");
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
