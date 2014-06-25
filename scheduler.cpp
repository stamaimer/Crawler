#include "scheduler.h"
#include "parser.h"

Scheduler::Scheduler()
{
}

void Scheduler::exec()
{
    timer.start();

//    requester.exec();

//    extractor.exec();

    files = pre_processor.exec();

    inserter = new Inserter();

    for(int i = 0; i < AMOUNT_OF_THREADS; ++i)
    {
        parsers[i] = new Parser(i, this);

        parsers[i]->start();
    }
}

void Scheduler::finished(int tid)
{
    static int count = 0;

    parsers[tid]->exit();

    if(AMOUNT_OF_THREADS == ++count)
    {
        qDebug() << "THREAD" << tid + AMOUNT_OF_THREADS << "FINISHED" << "TOTAL:" << count;

        qDebug() << "EXIT...";

        qDebug() << (double)(timer.elapsed() / 60000) << "min elapsed";

        delete inserter;

        exit(0);
    }
    else
    {
        qDebug() << "THREAD" << tid + AMOUNT_OF_THREADS << "FINISHED" << "TOTAL:" << count;

        qDebug() << (double)(timer.elapsed() / 60000) << "min elapsed";
    }
}
