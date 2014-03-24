#include <QCoreApplication>

#include <QUrl>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

#include <QEventLoop>

#include <QString>
#include <QByteArray>
#include <QStringList>

#include <QVector>

QByteArray mobile_device = "6E970F33-57E6-4F35-B729-C54B947AA3BE";
QByteArray mobile_os = "iPhone4S-7.0.4";
QByteArray login_name = "erightechgroup@gmail.com";
QByteArray password = "Adzhuang2267";
QByteArray ver_id = "2.5";

QByteArray cookie;

QVector<QString> family_ids;
QVector<QString> cat_ids;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if(QSslSocket::supportsSsl())
    {
        qDebug() << __TIME__ << __FUNCTION__ << "SSL enabled";
    }
    else
    {
        qDebug() << __TIME__ << __FUNCTION__ << "SSL unabled";
    }

    QNetworkReply* reply = NULL;

    QNetworkRequest request;

    QNetworkAccessManager manager;

    QEventLoop synchronous;

    QObject::connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));

    QString url = "https://ec.synnex.com/ec-mobile-new/mobile/account.do?method=resellerLogin";

    request.setUrl(QUrl(url));

    request.setRawHeader("content-type", "application/x-www-form-urlencoded");

    QByteArray request_body = "mobile_device=" + mobile_device + "&"
                              + "mobile_os=" + mobile_os + "&"
                              + "loginName=" + login_name + "&"
                              + "password=" + password + "&"
                              + "ver_id=" + ver_id;

    reply = manager.post(request, request_body);

    synchronous.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray set_cookie = reply->rawHeader("set-cookie");

        //qDebug() << __TIME__ << __FUNCTION__ << set_cookie;

        QList<QByteArray> fields = set_cookie.split(';');

        int size = fields.size();

        qDebug() << __TIME__ << __FUNCTION__ << size;

        cookie = fields[0];//global

        qDebug() << __TIME__ << __FUNCTION__ << cookie;
    }
    else
    {
        qDebug() << __TIME__ << __FUNCTION__ << reply->error() << reply->errorString();
    }

    url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=getCategories";

    request.setUrl(QUrl(url));

    request.setRawHeader("cookie", cookie + "; " + cookie);

    qDebug() << __TIME__ << __FUNCTION__ << cookie + "; " + cookie;

    request_body = "mobile_device=" + mobile_device + "&" + "mobile_os=" + mobile_os;

    reply = manager.post(request, request_body);

    synchronous.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();

        QJsonParseError parse_error;

        QJsonDocument doc = QJsonDocument::fromJson(response, &parse_error);

        if(parse_error.error == QJsonParseError::NoError)
        {
            QJsonObject obj = doc.object();

            QJsonArray data = obj["data"].toArray();

            for(int i = 0; i < data.size(); ++i)
            {
                QJsonObject family = data[i].toObject();

                family_ids.append(family["familyId"].toString());

                qDebug() << family["familyId"].toString() << family["familyDesc"].toString();

                QJsonArray sub_cats = family["subCats"].toArray();

                for(int j = 0; j < sub_cats.size(); ++j)
                {
                    cat_ids.append(sub_cats[j].toObject()["catId"].toString());

                    qDebug() << '\t' << sub_cats[j].toObject()["catId"].toString()
                                     << sub_cats[j].toObject()["catDesc"].toString();
                }
            }
        }
        else
        {
            qDebug() << __TIME__ << __FUNCTION__ << parse_error.error << parse_error.errorString();
        }
    }
    else
    {
        qDebug() << __TIME__ << __FUNCTION__ << reply->error() << reply->errorString();
    }

    url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=search";

    request.setUrl(QUrl(url));

    request.setRawHeader("cookie", cookie + "; " + cookie);

    qDebug() << __TIME__ << __FUNCTION__ << cookie + "; " + cookie;

    request_body = "mobile_device=6E970F33-57E6-4F35-B729-C54B947AA3BE&mobile_os=iPhone4S-7.1&available=false&begin=0&offset=400&familyId=26&catId=1154&spaType=VS&sortField=bestMatch&sortDirection=1";

    reply = manager.post(request, request_body);

    synchronous.exec();

    if(reply->error() == QNetworkReply::NoError)
    {
        QByteArray response = reply->readAll();

        QJsonParseError parse_error;

        QJsonDocument doc = QJsonDocument::fromJson(response, &parse_error);

        if(parse_error.error == QJsonParseError::NoError)
        {
            QJsonObject obj = doc.object();

            QJsonObject data = obj["data"].toObject();

            QJsonArray products = data["productList"].toArray();

            for(int i = 0; i < products.size(); ++i)
            {
                QJsonObject tmp = products[i].toObject();

                qDebug() << i << tmp["shortDescription"].toString()
                              << tmp["skuNo"].toString();
            }
        }
        else
        {
            qDebug() << __TIME__ << __FUNCTION__ << parse_error.error << parse_error.errorString();
        }
    }
    else
    {
        qDebug() << __TIME__ << __FUNCTION__ << reply->error() << reply->errorString();
    }


    return a.exec();
}
