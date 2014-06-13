#include "category.h"

Category::Category(QString id, QString name, QStringList path)
{
    this->id   = id;
    this->name = name;
    this->path = path;
}

QString Category::getId()
{
    return id;
}

QString Category::getName()
{
    return name.toHtmlEscaped().remove("\\");
}

Qstring Category::getPath()
{
    return path.join(',');
}
