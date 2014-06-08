#include "requester.h"

Requester::Requester()
{
}

void Requester::exec()
{
    QString pattern = "wget -O%1 http://api.remix.bestbuy.com/v1/%2.json.zip?apiKey=%3";

    exec(pattern.arg(CATEGORIES_ZIP_FILENAME).arg(CATEGORIES).arg(API_KEY));
    exec(pattern.arg(PRODUCTS_ZIP_FILENAME).arg(PRODUCTS).arg(API_KEY));
}

void Requester::exec(QString argument)
{
    int status;

    while(true)
    {
        status = QProcess::execute(argument);

        if(-1 == status)
        {
            qDebug() << "the process crashes.";
        }
        else if(-2 == status)
        {
            qDebug() << "the process can't be started.";
        }
        else
        {
            qDebug() << "the process' exit code is" << status;

            if(0 == status)
            {
                break;
            }
        }
    }
}
