#include "jobscheduler.h"
#include "sender.h"

const QByteArray JobScheduler::mobile_device = "6E970F33-57E6-4F35-B729-C54B947AA3BE";
const QByteArray JobScheduler::mobile_os = "iPhone4S-7.0.4";

const QByteArray JobScheduler::login_name = "erightechgroup@gmail.com";
const QByteArray JobScheduler::password = "Adzhuang2267";

const QByteArray JobScheduler::version_id = "2.5";

QByteArray JobScheduler::cookie = "";

JobScheduler::JobScheduler(QObject *parent) : QObject(parent)
{
    QString request_url = "https://ec.synnex.com/ec-mobile-new/mobile/account.do?method=resellerLogin";//经销商登录地址

    QMap<QByteArray, QByteArray> request_headers;

    request_headers["content-type"] = "application/x-www-form-urlencoded";

    QByteArray request_body = "mobile_device=" + mobile_device
                            + "&mobile_os="    + mobile_os
                            + "&loginName="    + login_name//违反命名规则的都是sb
                            + "&password="     + password
                            + "&ver_id="       + version_id;//违反命名规则的都是sb

    synnexs.append(Synnex(POST, request_url, request_headers, request_body, 0));

    for(int i = 0; i < AMOUNT_OF_THREAD; ++i)
    {
        senders[i] = new Sender(this);
        senders[i]->start(i + AMOUNT_OF_THREAD);//just for tidy
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

            //发生解析错误之时采取什么措施？

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

        cookie = fields[0] + "; " + fields[0];

        qDebug() << __TIME__ << __FUNCTION__ << cookie;

        //----------------------------------------------------------------------------------------------//

        QString request_url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=getCategories";

        QMap<QByteArray, QByteArray> request_headers;

        request_headers["cookie"] = JobScheduler::cookie;

        request_headers["content-type"] = "application/x-www-form-urlencoded";

        QByteArray request_body = "mobile_device=" + mobile_device + "&mobile_os=" + mobile_os;

        synnexs.append(Synnex(POST, request_url, request_headers, request_body, 1));

        //----------------------------------------------------------------------------------------------//
    }
}

void JobScheduler::getCategories(QNetworkReply* reply, Synnex synnex)
{
    if(getJsonDoc(reply, synnex, __FUNCTION__))
    {
        //----------------------------------------------------------------------------------------------//

        QString request_url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=search";

        QMap<QByteArray, QByteArray> request_headers;

        request_headers["cookie"] = JobScheduler::cookie;

        request_headers["content-type"] = "application/x-www-form-urlencoded";

        QByteArray request_body = "mobile_device=" + mobile_device + "&mobile_os=" + mobile_os
                                + "&begin=0&offset=30000&familyId=%1&catId=%2&spaType=VS";

        QString pattern = request_body;

        //----------------------------------------------------------------------------------------------//

        QJsonObject obj = doc.object();

        QJsonArray data = obj["data"].toArray();

        qDebug() << __TIME__ << __FUNCTION__;

        for(int i = 0; i < data.size(); ++i)
        {
            QJsonObject family = data[i].toObject();

            qDebug() << "\t\t"
                     << family["familyId"].toString()
                     << family["familyDesc"].toString();

            QJsonArray sub_cats = family["subCats"].toArray();

            for(int j = 0; j < sub_cats.size(); ++j)
            {
                qDebug() << "\t\t\t"
                         << sub_cats[j].toObject()["catId"].toString()
                         << sub_cats[j].toObject()["catDesc"].toString();

                //-------------------------------------------------------------------/

                request_body = pattern.arg(family["familyId"].toString())
                                      .arg(sub_cats[j].toObject()["catId"].toString())
                                      .toUtf8();

                synnexs.append(Synnex(POST, request_url, request_headers, request_body, 2));

                //-------------------------------------------------------------------/
            }
        }
    }
}

void JobScheduler::getSKU(QNetworkReply* reply, Synnex synnex)
{
    //----------------------------------------------------------------------------------------------//

    QString request_url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=getDetails";

    QMap<QByteArray, QByteArray> request_headers;

    request_headers["cookie"] = JobScheduler::cookie;

    request_headers["content-type"] = "application/x-www-form-urlencoded";

    QByteArray request_body = "mobile_device=" + mobile_device + "&mobile_os=" + mobile_os
                            + "&skuNo=%1&spaType=VS";

    QString pattern = request_body;

    //----------------------------------------------------------------------------------------------//

    if(getJsonDoc(reply, synnex, __FUNCTION__))
    {
        QJsonObject obj = doc.object();

        QJsonObject data = obj["data"].toObject();

        QString size = data["resultSize"].toString();

        qDebug() << size;
    }
}
