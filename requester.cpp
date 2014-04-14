#include "requester.h"

int Requester::count = 0;

Requester::Requester(int tid, JobScheduler* job_scheduler)
{
    this->tid           = tid;
    this->job_scheduler = job_scheduler;
}

void Requester::run()
{
    QEventLoop synchronous;

    QNetworkProxy         proxy;
    QNetworkRequest       request;
    QNetworkReply*        reply = NULL;
    QNetworkAccessManager manager;

    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("localhost");
    proxy.setPort(8888);

    QNetworkProxy::setApplicationProxy(proxy);

    connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));

    while(true)
    {
        job_scheduler->mutex.lock();

        if(500 == count)
        {
            qDebug() << "beg of insert merchandise";

            job_scheduler->timer.start();

            for(Merchandise merchandise : job_scheduler->merchandises)
            {
                job_scheduler->inserter->insert(merchandise);
            }

            count = 0;

            job_scheduler->merchandises.empty();

            qDebug() << job_scheduler->timer.elapsed() / 1000 << "elapsed";

            qDebug() << "end of insert merchandise";
        }

        if(job_scheduler->walmarts.size() != 0)
        {
            Walmart* walmart = job_scheduler->walmarts.first();

            job_scheduler->walmarts.remove(0);

            job_scheduler->mutex.unlock();

            request.setUrl(QUrl(walmart->request_url));

            reply = manager.get(request);

            synchronous.exec();

//            job_scheduler->mutex.lock();

            if(walmart->request_url.contains("taxonomy"))
            {
                job_scheduler->getMenus(reply, walmart);
            }
            else
            {
                if(job_scheduler->getMerchandise(reply, walmart))
                {
                    count++;
                }
            }

//            job_scheduler->mutex.unlock();
        }
        else
        {
            job_scheduler->mutex.unlock();
        }
    }
}
