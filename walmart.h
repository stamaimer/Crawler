#ifndef WALMART_H
#define WALMART_H

#include <QString>

class Walmart
{
public:
    Walmart();
    Walmart(QString, QString);
    Walmart(const Walmart&);
    Walmart operator = (const Walmart&);

    QString request_url;
    QString name;
};

#endif // WALMART_H
