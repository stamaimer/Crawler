#include "walmart.h"

Walmart::Walmart()
{
}

Walmart::Walmart(QString id, QString name, QString request_url)
{
    this->id          = id;
    this->name        = name;
    this->request_url = request_url;
}

Walmart::Walmart(const Walmart& that)
{
    this->id          = that.id;
    this->name        = that.name;
    this->request_url = that.request_url;
}

Walmart Walmart::operator =(const Walmart& that)
{
    return Walmart(that);
}
