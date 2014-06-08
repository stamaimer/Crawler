#ifndef EXTRACTOR_H
#define EXTRACTOR_H

#include <QString>

#include "utility.h"

#define CATEGORIES_DIRNAME "categories.json"
#define PRODUCTS_DIRNAME "products.json"

class Extractor
{
public:
    Extractor();
    void exec();
};

#endif // EXTRACTOR_H
