#include "jobscheduler.h"
#include "sender.h"

const QByteArray JobScheduler::mobile_os = "6E970F33-57E6-4F35-B729-C54B947AA3BE";
const QByteArray JobScheduler::mobile_device = "iPhone4S-7.0.4";

const QByteArray JobScheduler::login_name = "erightechgroup@gmail.com";
const QByteArray JobScheduler::password = "Adzhuang2267";

const QByteArray JobScheduler::version_id = "2.5";

JobScheduler::JobScheduler(QObject *parent) : QObject(parent)
{
    QString request_url = "https://ec.synnex.com/ec-mobile-new/mobile/account.do?method=resellerLogin";//经销商登录地址

    QMap<QByteArray, QByteArray> request_headers;

    request_headers["content-type"] = "application/x-www-form-urlencoded";

    QByteArray request_body = "mobile_device=" + mobile_device
                            + "&mobile_os=" + mobile_os
                            + "&loginName=" + login_name//违反命名规则的都是sb
                            + "&password="  + password
                            + "&ver_id="    + version_id;//违反命名规则的都是sb

    synnexs.append(Synnex(request_url, request_headers, request_body, 0));

    for(int i = 0; i < AMOUNT_OF_THREAD; ++i)
    {
        senders[i] = new Sender(this);
        senders[i]->start(i);
    }
}

bool JobScheduler::getJsonDoc(QNetworkReply* reply, Synnex synnex, QString FUNCTION)
{
    QByteArray response_body;

    if(reply->error() == QNetworkReply::NoError)
    {
        response_body = reply->readAll();

        doc = QJsonDocument::fromJson(response_body, &parse_error);

        if(parse_error.error == QJsonParseError::NoError)
        {
            return true;
        }
        else
        {
            qDebug() << __TIME__ << FUNCTION << parse_error.error << parse_error.errorString();

            //

            return false;
        }
    }
    else
    {
        qDebug() << __TIME__ << FUNCTION << reply->error() << reply->errorString();

        synnexs.append(synnex);

        return false;
    }
}

void JobScheduler::getCookie(QNetworkReply* reply, Synnex synnex)
{
    if(getJsonDoc(reply, synnex, __FUNCTION__))
    {
        QByteArray set_cookie;

        if(reply->hasRawHeader("set-cookie"))
        {
            set_cookie = reply->rawHeader("set-cookie");
        }

        QList<QByteArray> fields = set_cookie.split(';');

        JobScheduler::cookie = fields[0] + "; " + fields[0];

        qDebug() << __TIME__ << __FUNCTION__ << JobScheduler::cookie;
    }
}

void JobScheduler::killSender(int thread_id)
{
    qDebug() << "sender" << thread_id << "stopped";

    senders[thread_id]->deleteLater();
}
