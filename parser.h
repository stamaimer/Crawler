#ifndef PARSER_H
#define PARSER_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

#include <QStringList>

#include "utility.h"

class Parser
{
public:
    Parser();
    void exec();

private:
    void getCategories();
    void getProducts();
};

#endif // PARSER_H
