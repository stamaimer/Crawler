#include "requester.h"
#include "utils.h"

int Requester::final_menu_count = 0;

Requester::Requester(int tid, JobScheduler* job_scheduler):sleeped(false)
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

    connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));

    while(true)
    {
        job_scheduler->mutex.lock();

        if(1 == final_menu_count)
        {
            job_scheduler->timer.start();

            QSqlDatabase::database().transaction();

            job_scheduler->inserter->insert(job_scheduler->merchandises);

            QSqlDatabase::database().commit();

            final_menu_count = 0;

            job_scheduler->merchandises.empty();

            qDebug() << job_scheduler->timer.elapsed() << "elapsed";
        }

        if(job_scheduler->walmarts.size() != 0)
        {
            Walmart* walmart = job_scheduler->walmarts.first();

            job_scheduler->walmarts.remove(0);

            Utils::toggle(job_scheduler->ips, job_scheduler->proxy);

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
                if(job_scheduler->getMerchandise(reply, walmart))
                {
                    final_menu_count++;
                }
            }

            job_scheduler->mutex.unlock();
        }
        else
        {
            if(sleeped)
            {
                job_scheduler->mutex.unlock();

                break;
            }
            else
            {
                job_scheduler->mutex.unlock();

                sleep(10);

                sleeped = true;
            }
        }
    }
}
