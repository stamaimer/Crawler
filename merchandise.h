#ifndef MERCHANDISE_H
#define MERCHANDISE_H

#include <QObject>

#include <QString>
#include <QStringList>

class Merchandise
{
    QString upc;
    QString url;
    QString src;
    QString name;
    QString date;//上架日期
    double msrp;
    double price;
    bool stock;
    int reviews;
    QStringList path;

public:
    Merchandise(){}
    Merchandise(QString, QString, QString, QString, QString,
                double, double, bool, int, QStringList);

    QString getUPC();
    QString getURL();
    QString getSRC();
    QString getName();
    QString getDate();
    QString getPath();
    double getMSRP();
    double getPrice();
    bool getStock();
    int getReviews();
};

#endif // MERCHANDISE_H
