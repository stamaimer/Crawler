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
#include <QMainWindow>
#include <QTreeWidget>
#include <QTreeWidgetItem>

//网络相关
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkReply>
#include <QNetworkRequest>
#include <QNetworkAccessManager>

//解析相关
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>

class Spider : public QMainWindow
{
    Q_OBJECT

    QElapsedTimer timer;

    QTreeWidget* tree;

    QList<QTreeWidgetItem*> roots;//顶级目录结点
    QList<QTreeWidgetItem*> leaves;//次级目录结点

    QVector<int> menus;
    QVector<Category> categories;//调整类的结构
    QVector<Product> products;

    QNetworkAccessManager manager;
    QNetworkRequest request;
    QNetworkReply *reply;

    //是否应该这样编写
    QJsonParseError parse_error;
    QJsonDocument doc;

    //==============================
    /*volatile*/ bool isCategory;
    /*volatile*/ bool isSubCategory;
    //==============================

    void getMenus();
    void getCategories();
    void getSubCategories();
    void getPageCounts();
    void getProducts();

    void getJsonDoc(QNetworkReply*, QString);

public:
    Spider(QWidget* parent = 0);
    ~Spider();

private slots:
    void getMenus(QNetworkReply*);
    void getCategories(QNetworkReply*);
    void getSubCategories(QNetworkReply*);
    void getPageCounts(QNetworkReply*);
    void getProducts(QNetworkReply*);
};

#endif // SPIDER_H
