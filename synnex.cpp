#include "synnex.h"

Synnex::Synnex(){}

Synnex::Synnex(RequestMethod request_method, QString request_url, QMap<QByteArray, QByteArray> request_headers, QByteArray request_body, int level)
{
    this->request_method = request_method;

    this->request_url = request_url;

    this->request_headers = request_headers;

    this->request_body = request_body;

    this->level = level;
}

Synnex::Synnex(const Synnex& that)
{
    this->request_method = that.request_method;

    this->request_url = that.request_url;

    this->request_headers = that.request_headers;

    this->request_body = that.request_body;

    this->level = that.level;
}

Synnex Synnex::operator =(const Synnex& that)
{
    return Synnex(that);
}
