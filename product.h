#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>
#include <QStringList>

class Product
{
    QString upc;
    QString url;
    QString name;
    QString date;//上架日期
    double msrp;
    double price;
    bool stock;
    int reviews;
    QStringList path;

public:
    Product(){}
    Product(QString, QString, QString, QString,
                double, double, bool, int, QStringList);

    QString getUPC();
    QString getURL();
    QString getName();
    QString getDate();
    QString getPath();
    double getMSRP();
    double getPrice();
    int getStock();
    int getReviews();
};

#endif // PRODUCT_H
