#include "parser.h"

Parser::Parser()
{
}

void Parser::exec()
{
    exec(CATEGORIES_DIRNAME, CATEGORY);
//    exec(PRODUCTS_DIRNAME, PRODUCT);
}

void Parser::exec(QString dir_name, Type type)
{
    QDir dir = QDir(dir_name);

    dir.setNameFilters(QStringList("*.json"));

    QFileInfoList file_info_list = dir.entryInfoList();

    for(int i = 0; i < file_info_list.size(); ++i)
    {
//        qDebug() << file_info_list[i].fileName();

        QFile file(dir_name + "/" + file_info_list[i].fileName());

        if(!file.open(QIODevice::ReadOnly | QIODevice::Text))
        {
            qDebug() << "file open faild!!!";
        }

        QJsonParseError status;

        QJsonDocument doc = QJsonDocument::fromJson(file.readAll(), &status);

        if(QJsonParseError::NoError == status.error)
        {
            switch (type)
            {
                case CATEGORY:

                    dealWithCategories(doc);

                break;

                case PRODUCT:

                    dealWithProducts(doc);

                break;
            }
        }
        else
        {
            qDebug() << status.error << status.errorString();
        }
    }
}

void Parser::dealWithCategories(QJsonDocument doc)
{
    if(doc.isArray())
    {
        QJsonArray categories = doc.array();

        QJsonObject category;

        for(int i = 0; i < categories.size(); ++i)
        {
            category = categories[i].toObject();

            QString id = category["id"].toString();
            QString name = category["name"].toString();

            QJsonArray tmp = category["path"].toArray();

            QStringList path;

            for(int j = 0; j < tmp.size(); ++j)
            {
                path << tmp[j].toObject()["id"].toString();
            }

            qDebug() << id << name << path.join(',');
        }
    }
}

void Parser::dealWithProducts(QJsonDocument doc)
{

}
