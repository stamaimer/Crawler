#ifndef MERCHANDISE_H
#define MERCHANDISE_H

#include <QObject>

class Merchandise
{
    QString upc;
    QString sku;
    QString name;
public:
    explicit Merchandise();
};

#endif // MERCHANDISE_H
