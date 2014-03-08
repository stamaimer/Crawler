#ifndef SPIDER_H
#define SPIDER_H

#include "category.h"
#include "product.h"

//辅助功能：输出与计时
#include <QFile>
#include <QElapsedTimer>

//扩展数据类型
#include <QString>
#include <QByteArray>
#include <QStringList>

//容器
#include <QList>
#include <QVector>

//同步
#include <QEventLoop>

//控件
#include <QTextEdit>
#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>

//布局
#include <QHBoxLayout>
#include <QVBoxLayout>

//网络相关
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkProxy>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

//数据库相关
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlDatabase>

//解析相关
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

//多线程相关
#include <QMutex>
#include <QThread>

class Thread;

class Spider : public QWidget
{
    Q_OBJECT

    QElapsedTimer timer;

    QTextEdit* console;

    QTreeWidget* tree;

    QList<QTreeWidgetItem*> roots;//顶级目录结点
    QList<QTreeWidgetItem*> leaves;//次级目录结点

    QVector<int> menus;
    QVector<Category> categories;//调整类的结构
    QVector<Product> products;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    QSqlDatabase database;
    QSqlQuery query;

    //是否应该这样编写
    QJsonParseError parse_error;
    QJsonDocument doc;

    //==============================
    /*volatile*/ bool isCategory;
    /*volatile*/ bool isSubCategory;
    //==============================

    //=================
    Thread* threads[8];
    //=================

    void getMenus();
    void getCategories();
    void getSubCategories();
    void getPageCounts();
    void getProducts();

    void getJsonDoc(QNetworkReply*, QString);

    void initDatabase();

public:
    Spider(QWidget* parent = 0);
    ~Spider();

    //==================================
    QVector<QNetworkReply*> replys;

    QMutex mutex;

    void getProductsHandler(QByteArray);

    void getProducts(QNetworkReply*);
    //==================================

private slots:
    void getMenus(QNetworkReply*);
    void getCategories(QNetworkReply*);
    void getSubCategories(QNetworkReply*);
    void getPageCounts(QNetworkReply*);
    void getPageCounts(QNetworkReply*, int);//ADD FOR SYNCHRONOUS
    //void getProducts(QNetworkReply*);
    void addReply(QNetworkReply*);
};


class Thread : public QThread
{
    Spider* spider;

    int tid;

public:

    Thread(Spider* spider)
    {
        this->spider = spider;
    }

    void start(int tid)
    {
        this->tid = tid;

        QThread::start();

        qDebug() << "THREAD" << tid << "HAS BEEN SUBMITTED";
    }

    void run()
    {
        qDebug() << "THREAD" << tid << "IS RUNNING";

        while(true)
        {
            spider->mutex.lock();

            if(!spider->replys.isEmpty())
            {
                qDebug() << "REPLYS SIZE" << spider->replys.size();

                qDebug() << "THREAD" << tid << "IS SERVING";
                
                QNetworkReply* reply = spider->replys.takeFirst();
                
                spider->mutex.unlock();

                spider->getProducts(reply);

                qDebug() << "THREAD" << tid << "SERVE END";
            }
            else
            {
                spider->mutex.unlock();
            }
        }
    }
};

#endif // SPIDER_H
