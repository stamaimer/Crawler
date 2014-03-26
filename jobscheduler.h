#ifndef JOBSCHEDULER_H
#define JOBSCHEDULER_H

#include "synnex.h"

#include <QObject>

#include <QMutex>

#include <QVector>

class JobScheduler : public QObject
{
    Q_OBJECT

public:
    QMutex mutex;
    QVector<Synnex> synnexs;

    explicit JobScheduler(QObject *parent = 0);

signals:

public slots:

};

#endif // JOBSCHEDULER_H
