#include "scheduler.h"

Scheduler::Scheduler()
{
}

void Scheduler::exec()
{
    timer.start();

    Requester requester;

    requester.exec();

    Extractor extractor;

    extractor.exec();

    Parser parser;

    parser.exec();

    qDebug() << "time elapsed :" << timer.elapsed();
}
