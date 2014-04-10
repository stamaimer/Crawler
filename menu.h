#ifndef MENU_H
#define MENU_H

#include <QStringList>

class Menu
{
    QStringList parent_categories;

    QString name;

public:
    Menu(){}

    Menu(QString name, QStringList parent_categories)
    {
        this->name = name;
        this->parent_categories = parent_categories;
    }

    QString getName()
    {
        return name;
    }

    QStringList getParentCategories()
    {
        return parent_categories;
    }
};

#endif // MENU_H
