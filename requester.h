#ifndef REQUESTER_H
#define REQUESTER_H

#include <QProcess>

#include <QDebug>

#include "utility.h"

#define CATEGORIES categories
#define PRODUCTS products

class Requester
{
public:
    Requester();
    void exec();

private:
    void exec(QString);
};

#endif // REQUESTER_H
