#include "sender.h"

Sender::Sender(JobScheduler* job_scheduler)
{
    this->job_scheduler = job_scheduler;

    //set_stack_size?

    connect(this, SIGNAL(finished(int)), job_scheduler, SLOT());
}

void Sender::start(int thread_id)
{
    this->thread_id = thread_id;

    QThread::start();
}

void Sender::run()
{
    QEventLoop synchronous;

    QNetworkRequest request;
    QNetworkReply* reply = NULL;
    QNetworkAccessManager* manager = new QNetworkAccessManager();

    connect(manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));

    while(true)
    {
        job_scheduler.mutex.lock();

        if(job_scheduler.synnexs.size() != 0)
        {
            Synnex synnex = job_scheduler.synnexs.first();

            job_scheduler.synnexs.remove(0);

            job_scheduler.mutex.unlock();

            request.setUrl(QUrl(synnex.request_url));

            for(int i = 0; i < synnex.request_headers.size(); ++i)
            {
                request.setRawHeader(synnex.request_headers[i].);
            }
        }
    }
}



















