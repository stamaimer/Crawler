#ifndef UTILITY_H
#define UTILITY_H

#include <QProcess>
#include <QString>
#include <QDebug>

#define API_KEY "4pj6ws82bq85vafs2369bdeu"

#define CATEGORIES_ZIP_FILENAME "categories.json.zip"
#define PRODUCTS_ZIP_FILENAME "products.json.zip"
#define CATEGORIES_DIRNAME "categories.json"
#define PRODUCTS_DIRNAME "products.json"

class utility
{
public:
    static void exec(QString argument)
    {
        int status;

        while(true)
        {
            status = QProcess::execute(argument);

            if(-1 == status)
            {
                qDebug() << "the process crashes.\n";
            }
            else if(-2 == status)
            {
                qDebug() << "the process can't be started.\n";
            }
            else
            {
                qDebug() << "\nthe process' exit code is" << status << '\n';

                if(0 == status)
                {
                    break;
                }
            }
        }
    }
};

#endif // UTILITY_H
