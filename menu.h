#ifndef MENU_H
#define MENU_H

#include <QObject>

#include <QString>
#include <QStringList>

class Menu
{
    QString id;
    QString name;
    QStringList path;

public:
    Menu(QString, QString, QStringList);

    QString getId();
    QString getName();
    QString getPath();
};

#endif // MENU_H
