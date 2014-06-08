#ifndef UTILITY_H
#define UTILITY_H

#include <QProcess>
#include <QString>
#include <QDebug>

#define API_KEY "4pj6ws82bq85vafs2369bdeu"
#define CATEGORIES_ZIP_FILENAME "categories.json.zip"
#define PRODUCTS_ZIP_FILENAME "products.json.zip"

namespace utility
{
    void exec(QString argument)
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
}

#endif // UTILITY_H
