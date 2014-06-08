#include <QCoreApplication>

#include "requester.h"
#include "extractor.h"
#include "parser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

//    Requester requester;

//    requester.exec();

    Extractor extractor;

    extractor.exec();

    exit(0);

    return a.exec();
}
