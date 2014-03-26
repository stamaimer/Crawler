#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include <QObject>

class JobScheduler : public QObject
{
    Q_OBJECT
public:
    explicit JobScheduler(QObject *parent = 0);

signals:

public slots:

};

#endif // JOBSCHEDULER_H
