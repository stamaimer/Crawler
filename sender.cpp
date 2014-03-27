#include "sender.h"

Sender::Sender(JobScheduler* job_scheduler)
{
    this->job_scheduler = job_scheduler;

    //set_stack_size?
}

void Sender::start(int thread_id)
{
    this->thread_id = thread_id;

    QThread::start();
}

void Sender::run()
{
    QEventLoop synchronous;

    QNetworkProxy proxy;
    QNetworkRequest request;
    QNetworkReply* reply = NULL;
    QNetworkAccessManager manager;

    proxy.setType(QNetworkProxy::HttpProxy);
    proxy.setHostName("localhost");
    proxy.setPort(8888);
    QNetworkProxy::setApplicationProxy(proxy);

    connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));

    while(true)
    {
        job_scheduler->mutex.lock();

        if(job_scheduler->synnexs.size() != 0)
        {
            Synnex synnex = job_scheduler->synnexs.first();

            job_scheduler->synnexs.remove(0);

            job_scheduler->mutex.unlock();

            request.setUrl(QUrl(synnex.request_url));

            for(int i = 0; i < synnex.request_headers.size(); ++i)
            {
                request.setRawHeader(synnex.request_headers.keys()[i], synnex.request_headers.values()[i]);
            }

            reply = manager.post(request, synnex.request_body);

            synchronous.exec();

            job_scheduler->mutex.lock();

            switch (synnex.level)
            {
                case 0:
                    job_scheduler->getCookie(reply, synnex);
                break;
                case 1:
                    job_scheduler->getCategories(reply, synnex);
                break;
                case 2:

                break;
                case 3:

                break;
                default:

                break;
            }

            job_scheduler->mutex.unlock();
        }
        else
        {
            job_scheduler->mutex.unlock();
        }
    }
}



















