#include "spider.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Spider spider;

    spider.show();

    return a.exec();
}
