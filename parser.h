#ifndef PARSER_H
#define PARSER_H

#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QFileInfoList>

#include <QStringList>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

#include "utility.h"

enum Type{CATEGORY, PRODUCT};

class Parser
{
public:
    Parser();
    void exec();

private:
    void exec(QString, Type);
    void dealWithCategories(QJsonDocument);
    void dealWithProducts(QJsonDocument);
};

#endif // PARSER_H
