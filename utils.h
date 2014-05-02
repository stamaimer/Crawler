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

//class Utils
//{
//    //Q_OBJECT

//    static QNetworkRequest       request;
//    static QNetworkReply*        reply;
//    static QNetworkAccessManager manager;

//    static QEventLoop synchronous;

//public:

//    static void toggle(QNetworkProxy proxy)
//    {
//        request.setUrl(QUrl("http://www.iphai.com/getapi.ashx?ddh=633291215059316&num=1&port=&yys=1&am=0&guolv=y&mt=0&fm=text&spt=%"));

//        request.setRawHeader("user-agent", "stamaimer");

//        reply = manager.get(request);

//        synchronous.exec();

//        QByteArray reply_body;

//        if(reply->error() == QNetworkReply::NoError)
//        {
//            reply_body = reply->readAll();

//            qDebug() << reply_body;
//        }
//        else
//        {
//            qDebug() << reply->error() << reply->errorString();
//        }

//        QString ip   ;
//        QString port ;

//        proxy.setType(QNetworkProxy::HttpProxy);
//        proxy.setHostName(ip);
//        proxy.setPort(port.toInt());

//        qDebug() << "CURRENT" << proxy.hostName() << proxy.port();

//        QNetworkProxy::setApplicationProxy(proxy);
//    }
//};

//QEventLoop Utils::synchronous = QEventLoop();

//QNetworkRequest       Utils::request = QNetworkRequest();
//QNetworkReply*        Utils::reply = NULL;
//QNetworkAccessManager Utils::manager = QNetworkAccessManager();

#endif // UTILS_H
