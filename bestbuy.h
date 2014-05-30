#ifndef BESTBUY_H
#define BESTBUY_H

#include <QObject>

#include <QString>

class BestBuy : public QObject
{
    Q_OBJECT
public:
    explicit BestBuy(QObject *parent = 0);
             BestBuy(const BestBuy&);
             BestBuy(/*QString, QString, */QString, int);
             BestBuy operator = (const BestBuy&);

//    QString id;
//    QString name;
    QString request_url;
    int     request_count;

signals:

public slots:

};

#endif // BESTBUY_H
