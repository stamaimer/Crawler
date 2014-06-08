#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "requester.h"
#include "extractor.h"
#include "parser.h"

class Scheduler
{
public:
    Scheduler();
    void exec();
};

#endif // SCHEDULER_H
