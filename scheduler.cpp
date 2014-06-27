#include "scheduler.h"
#include "parser.h"

Scheduler::Scheduler()
{
}

void Scheduler::exec()
{
    timer.start();

    requester.exec();

    extractor.exec();

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

        rm();

        qDebug() << (double)(timer.elapsed() / 60000) << "min elapsed";
    }
}

void Scheduler::rm()
{
    rm(CATEGORIES_DIRNAME);
    rm(PRODUCTS_DIRNAME);
}

void Scheduler::rm(QString dir_name)
{
    QDir dir = QDir(dir_name);

    dir.setNameFilters(QStringList("*.json"));

    QFileInfoList file_info_list = dir.entryInfoList();

    foreach (QFileInfo file_info, file_info_list)
    {
        if(file_info.isFile())
        {
            file_info.dir().remove(file_info.fileName());
        }
        else if(file_info.isDir())
        {
            rm(file_info.absoluteFilePath());
        }
    }

    dir.rmpath(dir.absolutePath());
}




















