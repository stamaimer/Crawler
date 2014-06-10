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

    PreProcessor pre_processor;

    Parser parser = Parser(pre_processor.exec());

    parser.exec();

    qDebug() << "time elapsed :" << timer.elapsed();
}
