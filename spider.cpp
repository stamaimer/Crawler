#include "spider.h"

Spider::Spider(QWidget* parent) : QMainWindow(parent)
{
    timer.start();//启动计时

    tree = new QTreeWidget(this);//创建树形控件

    tree->resize(this->width(), this->height());//调整控件大小

    getMenus();
}





Spider::~Spider()
{
}





void Spider::getMenus()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMenus(QNetworkReply*)));//信号发射一次

    QString url = "http://www.ows.newegg.com/Stores.egg/ShopAllNavigation";

    request.setUrl(QUrl(url));

    manager.get(request);
}





void Spider::getCategories()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCategories(QNetworkReply*)));

    QString url = "http://www.ows.newegg.com/Stores.egg/StoreNavigation?storeId=%1&categoryId=-1&storeType=4&nodeId=-1";

    for(int i = 0; i < menus.size(); ++i)
    {
        request.setUrl(QUrl(url.arg(menus[i])));

        manager.get(request);
    }
}





void Spider::getSubCategories()
{
    //==============================================================================
    //QEventLoop synchronous;

    //connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));
    //==============================================================================

    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getSubCategories(QNetworkReply*)));

    QString url = "http://www.ows.newegg.com/Stores.egg/StoreNavigation?storeId=%1&categoryId=%2&storeType=%3&nodeId=%4";

    int size = categories.size();//获取二级目录个数

    for(int i = 0; i < size; ++i)
    {
        if(categories[i].isCategory())//判断二级目录与否
        {
            //qDebug() << categories[i].getDescription();

            isCategory = true;

            request.setUrl(QUrl(url.arg(categories[i].getStoreId())
                                   .arg(categories[i].getCategoryId())
                                   .arg(categories[i].getStoreType())
                                   .arg(categories[i].getNodeId())
                                )
                           );

            manager.get(request);

            //===========================
            //reply = manager.get(request);

            //synchronous.exec();

            //getSubCategories(reply);
            //===========================
        }
        else
        {
            //qDebug() << categories[i].getDescription();

            isCategory = false;

            emit manager.finished(reply);

            //======================
            //getSubCategories(reply);
            //======================
        }
    }
}





void Spider::getPageCounts()
{
    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getPageCounts(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");//设置请求头部信息

    request.setUrl(QUrl("http://www.ows.newegg.com/Search.egg/Query"));

    for(int i = 0; i < categories.size(); ++i)
    {
        if(categories[i].isCategory())
        {

        }
        else
        {
            QJsonObject json;//JSON对象

            //插入数据
            json.insert("NValue", categories[i].getNValue());
            json.insert("NodeId", categories[i].getNodeId());
            json.insert("StoreId", categories[i].getStoreId());
            json.insert("StoreType", categories[i].getStoreType());
            json.insert("SubCategoryId", categories[i].getSubCategoryId());

            QJsonDocument doc;

            doc.setObject(json);

            QByteArray request_body = doc.toJson();//转换格式

            manager.post(request, request_body);
        }
    }
}





void Spider::getProducts()
{

}





void Spider::getMenus(QNetworkReply* reply)//该函数调用一次
{
    disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMenus(QNetworkReply*)));//解绑信号关联

    getJsonDoc(reply, __FUNCTION__);

    //判断是否为空？

    if(doc.isArray())//判断数组与否
    {
        QJsonArray menus = doc.array();//转换数组

        for(int i = 0; i < menus.size(); ++i)
        {
            QString description = menus[i].toObject()["Description"].toString();//获取DESCRIPTION

            this->menus.append(menus[i].toObject()["StoreId"].toInt());//获取添加STOREID

            this->roots.append(new QTreeWidgetItem(QStringList(description))); //添加DESCRIPTION

            this->roots[i]->setCheckState(0, Qt::Unchecked);//设置选择状态
        }

        //删除MORE&AUTOPARTS
        this->menus.removeAt(13);
        this->roots.removeAt(13);
        this->menus.removeAt(13);
        this->roots.removeAt(13);

        tree->addTopLevelItems(roots);//添加顶级目录结点

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

        getCategories();
    }
    else
    {
        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
    }
}





void Spider::getCategories(QNetworkReply* reply)
{
    static int count = menus.size();//获得当前函数运行次数

    qDebug() << count;

    getJsonDoc(reply, __FUNCTION__);

    if(doc.isArray())//检查数组与否
    {
        //JSON

        QJsonArray array = doc.array();//临时数组

        //[{"":{}, "NavigationItemList":[{}*]}]

        QJsonObject obj = array[0].toObject();//临时对象

        //{"":{}, "NavigationItemList":[{}*]}

        QJsonArray categories = obj["NavigationItemList"].toArray();//次级目录列表

        //[{}*]

        QList<QTreeWidgetItem*> leaves;//临时使用

        for(int i = 0; i < categories.size(); ++i)
        {
            this->categories.append(Category(
                                             categories[i].toObject()["NodeId"].toInt(),
                                             categories[i].toObject()["StoreId"].toInt(),
                                             categories[i].toObject()["StoreType"].toInt(),
                                             categories[i].toObject()["CategoryId"].toInt(),
                                             categories[i].toObject()["SubCategoryId"].toInt(),
                                             categories[i].toObject()["NValue"].toInt(),
                                             categories[i].toObject()["PageCount"].toInt(),//NULL
                                             categories[i].toObject()["ShowSeeAllDeals"].toBool(),
                    categories[i].toObject()["Description"].toString()));//ADD FOR DEBUG IN 02/27/14

            QString description = categories[i].toObject()["Description"].toString();//获取DESCRIPTION

            QTreeWidgetItem* leaf = new QTreeWidgetItem(QStringList(description));//创建叶子

            leaves.append(leaf);//临时添加叶子

            this->leaves.append(leaf);
        }

        int index = menus.indexOf(QUrlQuery(reply->request().url()).queryItemValue("storeId").toInt());//获取索引

        roots[index]->addChildren(leaves);//每个顶级目录添加次级目录
    }
    else
    {
        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
    }

    if(--count)
    {
    }
    else
    {
        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getCategories(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

        getSubCategories();
    }
}





void Spider::getSubCategories(QNetworkReply* reply)
{
    static int count = categories.size();//获取当前函数运行次数
    static int size = categories.size();//获取二级目录大小用于遍历

    qDebug() << count;

    if(isCategory)
    {
        getJsonDoc(reply, __FUNCTION__);

        if(doc.isArray())//判断数组与否
        {
            //JSON

            QJsonArray array = doc.array();//临时数组

            if(array.size() != 0)//判空
            {
                //[{"":{}, "NavigationItemList":[{}*]}]

                QJsonObject obj = array[0].toObject();//临时对象

                //{"":{}, "NavigationItemList":[{}*]}

                QJsonArray categories = obj["NavigationItemList"].toArray();//次级目录列表

                //[{}*]

                QList<QTreeWidgetItem*> leaves;//临时使用

                for(int i = 0; i < categories.size(); ++i)
                {
                    this->categories.append(Category(
                                                     categories[i].toObject()["NodeId"].toInt(),
                                                     categories[i].toObject()["StoreId"].toInt(),
                                                     categories[i].toObject()["StoreType"].toInt(),
                                                     categories[i].toObject()["CategoryId"].toInt(),
                                                     categories[i].toObject()["SubCategoryId"].toInt(),
                                                     categories[i].toObject()["NValue"].toInt(),
                                                     categories[i].toObject()["PageCount"].toInt(),//NULL
                                                     categories[i].toObject()["ShowSeeAllDeals"].toBool(),
                            categories[i].toObject()["Description"].toString()));//ADD FOR DEBUG IN 02/27/14

                    QString description = categories[i].toObject()["Description"].toString();//获取DESCRIPTION

                    QTreeWidgetItem* leaf = new QTreeWidgetItem(QStringList(description));//创建叶子

                    leaves.append(leaf);//临时添加叶子

                    leaves[i]->setCheckState(0, Qt::Unchecked);//设置选择状态
                }

                //this->leaves[245 - count]->addChildren(leaves);//SYNCHRONOUS

                int index;

                //获取查询参数用于确定索引
                int node_id = QUrlQuery(reply->request().url()).queryItemValue("nodeId").toInt();
                int store_id = QUrlQuery(reply->request().url()).queryItemValue("storeId").toInt();
                int store_type = QUrlQuery(reply->request().url()).queryItemValue("storeType").toInt();
                int category_id = QUrlQuery(reply->request().url()).queryItemValue("categoryId").toInt();

                //遍历获取索引

                for(int i = 0; i < size; ++i)
                {
                    if(this->categories[i].getNodeId() == node_id)
                    {
                        if(this->categories[i].getStoreId() == store_id)
                        {
                            if(this->categories[i].getStoreType() == store_type)
                            {
                                if(this->categories[i].getCategoryId() == category_id)
                                {
                                    index = i;

                                    break;
                                }
                            }
                        }
                    }
                }

                this->leaves[index]->addChildren(leaves);//ASYNCHRONOUS
            }
            else
            {
                qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] CATEGORY IS NULL";
            }
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
    }
    else
    {
        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] IS NOT A CATEGORY";
    }

    if(--count)
    {
    }
    else
    {
        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getSubCategories(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

        //=============================================
        //输出所有目录(二级目录和三级目录)用于分析如何获得索引

        //for(int i = 0; i < categories.size(); ++i)
        //{
        //    qDebug() << categories[i].getDescription()
        //             << categories[i].isCategory()
        //             << categories[i].getStoreId()
        //             << categories[i].getCategoryId()
        //             << categories[i].getStoreType()
        //             << categories[i].getNodeId();
        //}
        //=============================================

        //getPageCounts();
    }
}





void Spider::getPageCounts(QNetworkReply* reply)
{
    getJsonDoc(reply, __FUNCTION__);
}





void Spider::getProducts(QNetworkReply* reply)
{
    getJsonDoc(reply, __FUNCTION__);
}





void Spider::getJsonDoc(QNetworkReply* reply, QString FUNCTION)
{
    if(reply->error() == QNetworkReply::NoError)//检查是否发生网络错误
    {
        QByteArray response = reply->readAll();//获取响应信息同时清空

        doc = QJsonDocument::fromJson(response, &parse_error);//解析响应信息

        if(parse_error.error == QJsonParseError::NoError)//检查是否发生解析错误
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] SUCCESS";
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] PARSE ERROR";
        }
    }
    else
    {
        qDebug() << __TIME__ "IN [" << FUNCTION << "] NETWORK ERROR";
    }
}





























