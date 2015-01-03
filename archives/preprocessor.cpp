#include "preprocessor.h"

PreProcessor::PreProcessor()
{
}

QVector<QString> PreProcessor::exec()
{
    return exec(CATEGORIES_DIRNAME) + exec(PRODUCTS_DIRNAME);
}

QVector<QString> PreProcessor::exec(QString dir_name)
{
    QDir dir = QDir(dir_name);

    dir.setNameFilters(QStringList("*.json"));

    QFileInfoList file_info_list = dir.entryInfoList();

    QVector<QString> files;

    for(int i = 0; i < file_info_list.size(); ++i)
    {
//        qDebug() << file_info_list[i].fileName();

        files.append(dir_name + "/" + file_info_list[i].fileName());
    }

    return files;
}
