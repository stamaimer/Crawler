#include "scheduler.h"

Scheduler::Scheduler()
{
}

void Scheduler::exec()
{

    Requester requester;

    requester.exec();

    Extractor extractor;

    extractor.exec();

    Parser parser;

    parser.exec();

    qDebug() << "all task finished";
}
