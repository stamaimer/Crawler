#include "parser.h"

Parser::Parser()
{
}

void Parser::exec()
{
    getCategories();
}

void Parser::getCategories()
{
    QDir categories_dir = QDir(CATEGORIES_DIRNAME);

    categories_dir.setNameFilters(QStringList("*.json"));

    QFileInfoList fileinfos = categories_dir.entryInfoList();

    for(int i = 0; i < fileinfos.size(); ++i)
    {
        //deal with categories
    }
}

void Parser::getProducts()
{
    QDir products_dir = QDir(PRODUCTS_DIRNAME);

    products_dir.setNameFilters(QStringList("*.json"));

    QFileInfoList fileinfos = products_dir.entryInfoList();

    for(int i = 0; i < fileinfos.size(); ++i)
    {
        //deal with products
    }
}
