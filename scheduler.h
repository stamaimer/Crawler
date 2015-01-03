#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <QElapsedTimer>

#include <QObject>

#include <QMutex>

#include <QDate>

#include <QDir>
#include <QFile>
#include <QFileInfoList>

#include "preprocessor.h"
#include "requester.h"
#include "extractor.h"
#include "inserter.h"
#include "utility.h"

#define AMOUNT_OF_THREADS 5

class Parser;

class Scheduler : public QObject
{
    Q_OBJECT

    QElapsedTimer timer;

    Requester requester;

    Extractor extractor;

    PreProcessor pre_processor;

    Parser* parsers[AMOUNT_OF_THREADS];

    void rm();

    void mv();

    void rm(QString);

    void mv(QString);

public:

    Scheduler();

    void exec();

    Inserter* inserter;

    QMutex mutex;

    QVector<QString> files;

public slots:

    void finished(int);
};

#endif // SCHEDULER_H
