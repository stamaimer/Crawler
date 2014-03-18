#ifndef SPIDER_H
#define SPIDER_H

#include "category.h"
#include "product.h"
#include "packet.h"

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

class PThread;
class CThread;

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
    QVector<Category> sub_categories;
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

    //===================
    CThread* cthreads[1];
    PThread* pthreads[100];
    //===================

    void getMenus();
    void getCategories();
    void getSubCategories();
    void getPageCounts();
    //void getProducts();//这个函数移至生产者线程

    int getJsonDoc(QNetworkReply*, QString);
    int getJsonDoc(QNetworkReply*, Packet, QString);

    void initDatabase();

    void contract();

public:
    Spider(QWidget* parent = 0);
    ~Spider();

    //==================================================================
    QVector<Packet> packets;//PThread

    QVector<QNetworkReply*> replys;//CThread

    QMutex mutex;

    void getProducts(QNetworkReply*, Packet);//CThread

    void redirect(QtMsgType, const QMessageLogContext&, const QString&);
    //==================================================================

private slots:
    void getMenus(QNetworkReply*);
    void getCategories(QNetworkReply*);
    void getSubCategories(QNetworkReply*);
    void getPageCounts(QNetworkReply*);
    void getPageCounts(QNetworkReply*, int);//ADD FOR SYNCHRONOUS
    //void getProducts(QNetworkReply*);//这个函数移至消费者线程
};


class CThread : public QThread
{
    Q_OBJECT

    Spider* spider;

    int tid;

public:

    CThread(Spider* spider)
    {
        this->spider = spider;
        this->setStackSize(10240);
    }

    void start(int tid)
    {
        this->tid = tid;

        QThread::start();
    }

    void run()
    {
        while(true)
        {
            spider->mutex.lock();

            if(!spider->replys.isEmpty())
            {
                //QNetworkReply* reply = spider->replys.takeFirst();

                //spider->getProducts(reply);

                spider->mutex.unlock();
            }
            else
            {
                spider->mutex.unlock();
            }
        }
    }
};

class PThread : public QThread
{
    Q_OBJECT

    Spider* spider;

    int tid;

public:
    PThread(Spider* spider)
    {
        this->spider = spider;
        this->setStackSize(10240);
    }

    void start(int tid)
    {
        this->tid = tid;

        QThread::start();
    }

    void run()
    {
        QNetworkAccessManager* manager = new QNetworkAccessManager();

        QNetworkRequest request;

        QNetworkReply* reply;

        while(true)
        {
            spider->mutex.lock();

            if(!spider->packets.isEmpty())
            {
                Packet packet = spider->packets.takeFirst();

                spider->mutex.unlock();

                //connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addReply(QNetworkReply*)));

                //==============================================================================
                QEventLoop synchronous;

                connect(manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));
                //==============================================================================

                request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

                request.setUrl(QUrl("http://www.ows.newegg.com/Search.egg/Query"));

                for(int page_number = packet.getBegin(); page_number < packet.getEnd(); ++page_number)
                {
                    QJsonObject json;

                    //插入数据
                    json.insert("NValue", packet.getNValue());
                    json.insert("NodeId", packet.getNodeId());
                    json.insert("StoreId", packet.getStoreId());
                    json.insert("StoreType", packet.getStoreType());
                    json.insert("PageNumber", page_number + 1);//插入页码信息
                    json.insert("SubCategoryId", packet.getSubCategoryId());

                    QJsonDocument doc;

                    doc.setObject(json);

                    QByteArray request_body = doc.toJson();//转换数据

                    reply = manager->post(request, request_body);

                    synchronous.exec();

                    spider->mutex.lock();

                    //spider->replys.append(reply);

                    spider->getProducts(reply, packet);

                    spider->mutex.unlock();

                    qDebug() << "PTHREAD" << tid << "serves" << packet.getDescription();
                }
            }
            else
            {
                spider->mutex.unlock();

                break;
            }
        }
    }

public slots:

    void addReply(QNetworkReply* reply)
    {
        spider->mutex.lock();

        spider->replys.append(reply);

        spider->mutex.unlock();
    }

};

#endif // SPIDER_H
