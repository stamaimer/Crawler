#ifndef MENU_H
#define MENU_H

#include <QString>
#include <QStringList>

class Menu
{
    QString id;

    QString name;

    QString category;

    QStringList parent_categories;

public:
    Menu(){}

    Menu(QString id, QString name, QString category, QStringList parent_categories)
    {
        this->id = id;
        this->name = name;
        this->category = category;
        this->parent_categories = parent_categories;
    }

    QString getId()
    {
        return id;
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
