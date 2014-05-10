#include "requester.h"

int Requester::request_count    = 0;

Requester::Requester(int tid, JobScheduler* job_scheduler):sleeped(false), sleep_time(10)
{
    this->tid           = tid;
    this->job_scheduler = job_scheduler;
}

void Requester::run()
{
    QEventLoop synchronous;

    QNetworkRequest       request;
    QNetworkReply*        reply = NULL;
    QNetworkAccessManager manager;

    connect(this, SIGNAL(finished(int)), job_scheduler, SLOT(finished(int)), Qt::DirectConnection);
    connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));

    while(true)
    {
        job_scheduler->mutex.lock();

        if(500 == request_count)
        {
            qDebug() << "INSERT AND SLEEP";

            QSqlDatabase::database().transaction();

            job_scheduler->inserter->insert(job_scheduler->merchandises);

            QSqlDatabase::database().commit();

            job_scheduler->merchandises.clear();

            request_count = 0;
        }

        if(job_scheduler->walmarts.size() != 0)
        {
            Walmart* walmart = job_scheduler->walmarts.first();

            job_scheduler->walmarts.remove(0);

            job_scheduler->mutex.unlock();

            request.setUrl(QUrl(walmart->request_url));

            reply = manager.get(request);

            synchronous.exec();

            job_scheduler->mutex.lock();

            if(walmart->request_url.contains("taxonomy"))
            {
                job_scheduler->getMenus(reply, walmart);
            }
            else
            {
                job_scheduler->getMerchandise(reply, walmart);
            }

            request_count++;

            job_scheduler->mutex.unlock();
        }
        else
        {
            job_scheduler->mutex.unlock();

//            if(sleeped)
//            {
//                break;
//            }
//            else
//            {
//                sleeped = true;

//                sleep(SLEEP_TIME);
//            }

            if(20 == sleep_time)
            {
                break;
            }
            else
            {

                sleep(sleep_time);

                sleep_time++;
            }
        }
    }

    emit finished(tid);
}
