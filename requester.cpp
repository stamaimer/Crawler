#include "requester.h"

Requester::Requester()
{
}

void Requester::exec()
{
    QString pattern = "wget -O%1 http://api.remix.bestbuy.com/v1/%2.json.zip?apiKey=%3";

    utility::exec(pattern.arg(CATEGORIES_ZIP_FILENAME).arg(CATEGORIES).arg(API_KEY));
//    utility::exec(pattern.arg(PRODUCTS_ZIP_FILENAME).arg(PRODUCTS).arg(API_KEY));
}

