#ifndef UTILS_H
#define UTILS_H

#include <QVector>
#include <QString>

#include <QNetworkProxy>

#include <stdlib.h>
#include <time.h>

class Utils
{
public:

    static void toggle(QVector<QString> ips, QNetworkProxy proxy)
    {
        srand(time(NULL));

        int index = rand() % ips.size();

        qDebug() << ips.size() << index << ips[index];

        QString ip   = ips[index].split(':')[0];
        QString port = ips[index].split(':')[1];

        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(ip);
        proxy.setPort(port.toInt());

        QNetworkProxy::setApplicationProxy(proxy);

    }
};

#endif // UTILS_H
