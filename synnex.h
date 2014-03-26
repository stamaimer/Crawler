#ifndef SYNNEX_H
#define SYNNEX_H

#include <QObject>

#include <QString>
#include <QByteArray>

#include <QMap>
#include <QVector>
#include <QStringList>


#include <QElapsedTimer>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

class Synnex : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

    static QByteArray mobile_os = "6E970F33-57E6-4F35-B729-C54B947AA3BE";
    static QByteArray mobile_device = "iPhone4S-7.0.4";

    static QByteArray login_name = "erightechgroup@gmail.com";
    static QByteArray password = "Adzhuang2267";

    static QByteArray version_id = "2.5";

    QByteArray cookie;

    QString request_url;
    QMap<QByteArray, QByteArray> request_headers;
    QString request_body;

    bool login();

public:
    explicit Synnex(QObject *parent = 0);

signals:

public slots:

};

#endif // SYNNEX_H
