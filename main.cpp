#include <QCoreApplication>
#include <requester.h>
#include <QDebug>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    Requester requester;

    requester.exec();

    exit(0);

    return a.exec();
}
