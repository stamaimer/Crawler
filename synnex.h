#ifndef SYNNEX_H
#define SYNNEX_H

#include <QObject>

#include <QMap>

#include <QString>
#include <QByteArray>

class Synnex : public QObject
{
    Q_OBJECT

public:

    QString request_url;
    QMap<QByteArray, QByteArray> request_headers;
    QByteArray request_body;

    int level;//the variable name might be changed

public:
    Synnex();

    explicit Synnex(QString, QMap<QByteArray, QByteArray>, QByteArray, int);

    Synnex(const Synnex&);

    Synnex operator =(const Synnex&);

signals:

public slots:

};

#endif // SYNNEX_H
