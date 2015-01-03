#include "bestbuy.h"

BestBuy::BestBuy(QObject *parent) : QObject(parent)
{
}

BestBuy::BestBuy(/*QString id, QString name, */QString request_url, int request_count)
{
//    this->id            = id;
//    this->name          = name;
    this->request_url   = request_url;
    this->request_count = request_count;
}

BestBuy::BestBuy(const BestBuy& that)
{
//    this->id            = that.id;
//    this->name          = that.name;
    this->request_url   = that.request_url;
    this->request_count = that.request_count;
}

BestBuy BestBuy::operator =(const BestBuy& that)
{
    return BestBuy(that);
}
