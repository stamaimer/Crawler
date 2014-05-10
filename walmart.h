#ifndef WALMART_H
#define WALMART_H

#include <QString>

class Walmart
{
public:
    Walmart();
    Walmart(const Walmart&);
    Walmart(QString, QString, QString, int);
    Walmart operator = (const Walmart&);

    QString id;
    QString name;
    QString request_url;
    int     request_count;
};

#endif // WALMART_H
