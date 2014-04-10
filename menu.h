#ifndef MENU_H
#define MENU_H

#include <QStringList>

class Menu
{
    QString name;

    QString category;

    QStringList parent_categories;

public:
    Menu(){}

    Menu(QString name, QString category, QStringList parent_categories)
    {
        this->name = name;
        this->category = category;
        this->parent_categories = parent_categories;
    }

    QString getName()
    {
        return name;
    }

    QString getCategory()
    {
        return category;
    }

    QStringList getParentCategories()
    {
        return parent_categories;
    }
};

#endif // MENU_H
