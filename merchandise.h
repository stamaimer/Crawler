#ifndef MERCHANDISE_H
#define MERCHANDISE_H

#include <QObject>

#include <QString>
#include <QStringList>

class Merchandise
{
    QString upc;
    QString sku;
    QString url;
    QString name;
    QString date;//上架日期
    QString msrp;
    QString price;
    QString stock;
    QString reviews;
    QStringList path;

public:
    Merchandise(QString, QString, QString,
                QString, QString, QString,
                QString, QString, QString,
                QStringList);

    QString getUPC();
    QString getSKU();
    QString getURL();
    QString getName();
    QString getDate();
    QString getMSRP();
    QString getPath();
    QString getPrice();
    QString getStock();
    QString getReviews();
};

#endif // MERCHANDISE_H
