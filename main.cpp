#include <QCoreApplication>

QVector<QString> family_ids;
QVector<QString> cat_ids;
QVector<QString> sku_nos;

QVector<int> temp;

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


            QString pattern = "mobile_device=6E970F33-57E6-4F35-B729-C54B947AA3BE&mobile_os=iPhone4S-7.1&begin=0&offset=10&familyId=%1&catId=%2&spaType=VS";

            for(int i = 0; i < data.size(); ++i)
            {
                QJsonObject family = data[i].toObject();

                family_ids.append(family["familyId"].toString());

                QString family_id = family["familyId"].toString();

                qDebug() << family["familyId"].toString() << family["familyDesc"].toString();

                QJsonArray sub_cats = family["subCats"].toArray();

                for(int j = 0; j < sub_cats.size(); ++j)
                {
                    cat_ids.append(sub_cats[j].toObject()["catId"].toString());

                    QString cat_id = sub_cats[j].toObject()["catId"].toString();

                    qDebug() << '\t' << sub_cats[j].toObject()["catId"].toString()
                                     << sub_cats[j].toObject()["catDesc"].toString();

                    QString tmp = pattern.arg(family_id).arg(cat_id);

                    //qDebug() << tmp;

                    request_body = tmp.toUtf8();

                    //qDebug() << request_body;

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

                            QString size = data["resultSize"].toString();

                            temp.append(size.toInt());

                            qDebug() << '\t' << size;
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

    selection_sort(temp);

    qDebug() << temp.first();

//    url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=search";

//    request.setUrl(QUrl(url));

//    request.setRawHeader("cookie", cookie + "; " + cookie);

//    qDebug() << __TIME__ << __FUNCTION__ << cookie + "; " + cookie;

    //request_body = "mobile_device=6E970F33-57E6-4F35-B729-C54B947AA3BE&mobile_os=iPhone4S-7.1&available=false&begin=0&offset=400&familyId=26&catId=1154&spaType=VS&sortField=bestMatch&sortDirection=1";

//    QString pattern = "mobile_device=6E970F33-57E6-4F35-B729-C54B947AA3BE&mobile_os=iPhone4S-7.1&begin=0&offset=10&familyId=%1&catId=%2&spaType=VS";





//    url = "http://ec.synnex.com/ec-mobile-new/mobile/product.do?method=getDetails";

//    request.setUrl(QUrl(url));

//    request.setRawHeader("cookie", cookie + "; " + cookie);

//    qDebug() << __TIME__ << __FUNCTION__ << cookie + "; " + cookie;

//    request_body = "mobile_device=6E970F33-57E6-4F35-B729-C54B947AA3BE&mobile_os=iPhone4S-7.1&skuNo=140947&spaType=VS";

//    reply = manager.post(request, request_body);

//    synchronous.exec();

//    if(reply->error() == QNetworkReply::NoError)
//    {
//        QByteArray response = reply->readAll();

//        QJsonParseError parse_error;

//        QJsonDocument doc = QJsonDocument::fromJson(response, &parse_error);

//        if(parse_error.error == QJsonParseError::NoError)
//        {
//            QJsonObject obj = doc.object();

//            QJsonObject data = obj["data"].toObject();

//            QJsonObject product = data["product"].toObject();

//            qDebug() << product["shortDescription"].toString()
//                     << product["skuNo"].toString()
//                     << product["UPC"].toString()
//                     << product["Weight"].toString()
//                     << product["price"].toObject()["priceDisplay"].toString()
//                     << product["avail"].toObject()["availDisplay"].toString();
//        }
//        else
//        {
//            qDebug() << __TIME__ << __FUNCTION__ << parse_error.error << parse_error.errorString();
//        }
//    }
//    else
//    {
//        qDebug() << __TIME__ << __FUNCTION__ << reply->error() << reply->errorString();
//    }


    return a.exec();
}

void selection_sort(QVector<int> temp)
{
    int index = 0;//index保存单趟比较过程中最大元素下标

    for(int trip = 0; trip < temp.size() - 1; ++trip)//最后一个元素不比
    {
        index = trip;//使用趟数初始化index

        for(int cursor = trip + 1; cursor < temp.size(); ++cursor)
        {
            if(temp[index] < temp[cursor])
            {
                index = cursor;
            }
        }

        swap(temp[trip], temp[index]);
    }
}

void swap(int& a, int& b)
{
    int tmp = a;
    a = b;
    b = tmp;
}
