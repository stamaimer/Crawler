#ifndef WALMART_H
#define WALMART_H

#include <QString>

class Walmart
{
public:
    Walmart();
    Walmart(const Walmart&);
    Walmart(QString, QString);
    Walmart operator = (const Walmart&);

    QString name;
    QString request_url;
};

#endif // WALMART_H
