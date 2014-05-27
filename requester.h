#ifndef REQUESTER_H
#define REQUESTER_H

#include <QObject>

class requester : public QObject
{
    Q_OBJECT
public:
    explicit requester(QObject *parent = 0);

signals:

public slots:

};

#endif // REQUESTER_H
