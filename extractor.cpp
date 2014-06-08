#include "extractor.h"

Extractor::Extractor()
{
}

void Extractor::exec()
{
    QString pattern = "7z e -o%1 %2";

    utility::exec(pattern.arg(CATEGORIES_DIRNAME).arg(CATEGORIES_ZIP_FILENAME));
//    utility::exec(pattern.arg(PRODUCTS_DIRNAME).arg(PRODUCTS_ZIP_FILENAME));
}
