#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "utility.h"

#include <QDir>
#include <QFileInfoList>

#include <QStringList>
#include <QString>
#include <QVector>

class PreProcessor
{
public:
    PreProcessor();
    QVector<QString> exec();

private:
    QVector<QString> exec(QString);
};

#endif // PREPROCESSOR_H
