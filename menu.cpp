#include "menu.h"

Menu::Menu(QString id, QString name, QStringList path)
{
    this->id   = id;
    this->name = name;
    this->path = path;
}

QString Menu::getId()
{
    return id;
}

QString Menu::getName()
{
    return name.toHtmlEscaped().remove("\\");
}

QString Menu::getPath()
{
    return path.join(">>");
}
