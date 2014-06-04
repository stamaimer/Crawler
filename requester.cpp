#include "requester.h"

int Requester::send_count = 0;

Requester::Requester(int tid, QString apikey, JobScheduler* job_scheduler):sleep_time(SLEPP_TIME)
{
    this->tid           = tid;
    this->apikey        = apikey;
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

//        if(500 < send_count)
//        {
//            qDebug() << "INSERTING...";

//            QSqlDatabase::database().transaction();

//            job_scheduler->inserter->insert(job_scheduler->menus);
//            job_scheduler->inserter->insert(job_scheduler->merchandises);

//            QSqlDatabase::database().commit();

//            job_scheduler->menus.clear();
//            job_scheduler->merchandises.clear();

//            send_count = 0;
//        }

        if(job_scheduler->bestbuys.size() != 0)
        {
            BestBuy* bestbuy = job_scheduler->bestbuys.first();

            job_scheduler->bestbuys.remove(0);

            job_scheduler->mutex.unlock();

            request.setUrl(QUrl(bestbuy->request_url.arg(apikey)));

            reply = manager.get(request);

            synchronous.exec();

            job_scheduler->mutex.lock();

            if(bestbuy->request_url.contains("categories"))
            {
                job_scheduler->getMenus(reply, bestbuy);
            }
            else
            {
                job_scheduler->getMerchandises(reply, bestbuy);
            }

            send_count++;

            job_scheduler->mutex.unlock();
        }
        else
        {
            job_scheduler->mutex.unlock();

            if(MAX_SLEEP_TIME == sleep_time)
            {
                break;
            }
            else
            {
                sleep(sleep_time++);
            }
        }
    }

    emit finished(tid);
}
