#include "scheduler.h"
#include "parser.h"

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

    files = pre_processor.exec();

    Parser* parsers[AMOUNT_OF_THREADS];

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        parsers[i] = new Parser(i, this);

        parsers[i]->start();
    }

    qDebug() << "time elapsed :" << timer.elapsed();
}
