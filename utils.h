#ifndef UTILS_H
#define UTILS_H

#include <QObject>
#include <QVector>
#include <QString>

#include <QEventLoop>

#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

class Utils : public QObject
{
    Q_OBJECT

    QNetworkProxy         proxy;
    QNetworkReply*        reply;
    QNetworkRequest       request;
    QNetworkAccessManager manager;

    QEventLoop synchronous;

public:
    Utils()
    {
        connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));
    }

    void toggle()
    {
        request.setUrl(QUrl("http://net.iphai.com/getapi.ashx?ddh=633291215059316&num=1&yys=6&am=3&guolv=y&mt=6&fm=text"));

        request.setRawHeader("user-agent", "stamaimer");

        reply = manager.get(request);

        synchronous.exec();

        QByteArray reply_body;

        if(reply->error() == QNetworkReply::NoError)
        {
            reply_body = reply->readAll();

//            qDebug() << reply_body;
        }
        else
        {
            qDebug() << reply->error() << reply->errorString();
        }

        QString ip   = reply_body.split(':')[0];
        QString port = reply_body.split(':')[1];

        proxy.setType(QNetworkProxy::HttpProxy);
        proxy.setHostName(ip);
        proxy.setPort(port.toInt());

        qDebug() << "CURRENT" << proxy.hostName() << proxy.port();

        QNetworkProxy::setApplicationProxy(proxy);
    }
};

#endif // UTILS_H
