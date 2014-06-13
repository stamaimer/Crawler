#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>
#include <QStringList>

class Category
{
    QString id;
    QString name;
    QStringList path;

public:
    Category(){}
    Category(QString, QString, QStringList);

    QString getId();
    QString getName();
    QString getPath();
};

#endif // CATEGORY_H
