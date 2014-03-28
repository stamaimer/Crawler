#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "utils.h"
#include "synnex.h"

#include <QObject>

#include <QMutex>

#include <QNetworkReply>

#include <QDebug>
#include <QElapsedTimer>

#include <QString>
#include <QByteArray>

#include <QList>
#include <QVector>
#include <QStringList>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#define AMOUNT_OF_THREAD 10

class Sender;

class JobScheduler : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

    static const QByteArray mobile_os;
    static const QByteArray mobile_device;

    static const QByteArray login_name;
    static const QByteArray password;

    static const QByteArray version_id;

    static QByteArray cookie;

    Sender* senders[AMOUNT_OF_THREAD];

    QJsonParseError parse_error;
    QJsonDocument doc;

    bool getJsonDoc(QNetworkReply*, Synnex, QString);

public:
    QMutex mutex;
    QVector<Synnex> synnexs;

    explicit JobScheduler(QObject *parent = 0);

signals:

public slots:
    void getSKU(QNetworkReply*, Synnex);
    void getCookie(QNetworkReply*, Synnex);
    void getCategories(QNetworkReply*, Synnex);
};

#endif // JOBSCHEDULER_H
