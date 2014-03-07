#include "spider.h"

Spider::Spider(QWidget* parent) : QWidget(parent)
{
    timer.start();//启动计时

    tree = new QTreeWidget(this);//创建树形控件

    QHBoxLayout *layout = new QHBoxLayout();//创建水平布局

    layout->addWidget(tree);//添加树形控件

    this->setLayout(layout);//设置布局

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

    this->menus.remove(this->menus.indexOf(6));     //REMOVE SOFTWARE
    this->menus.remove(this->menus.indexOf(140));   //REMOVE CELL_PHONE
    this->menus.remove(this->menus.indexOf(15));    //REMOVE HOME&OUTDOORS
    this->menus.remove(this->menus.indexOf(19));    //REMOVE AOTOMOTIVE
    this->menus.remove(this->menus.indexOf(131));   //REMOVE ACCESSORIES
    this->menus.remove(this->menus.indexOf(143));   //REMOVE SERVICES
    this->menus.remove(this->menus.indexOf(14));    //REMOVE MARKETPLACE

    //ADD FOR TEST IN 03/06/14
    //========================
    //this->menus.clear();

    //this->menus.append(1);
    //========================

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
    //==============================================================================
    QEventLoop synchronous;

    connect(&manager, SIGNAL(finished(QNetworkReply*)), &synchronous, SLOT(quit()));
    //==============================================================================

    //connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getPageCounts(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");//设置请求头部信息

    request.setUrl(QUrl("http://www.ows.newegg.com/Search.egg/Query"));

    for(int i = 0; i < categories.size(); ++i)
    {
        if(categories[i].isCategory())
        {
            isSubCategory = false;

            //emit manager.finished(reply);

            //======================
            getPageCounts(reply, i);
            //======================
        }
        else
        {
            isSubCategory = true;

            QJsonObject json;

            //插入数据
            json.insert("NValue", categories[i].getNValue());
            json.insert("NodeId", categories[i].getNodeId());
            json.insert("StoreId", categories[i].getStoreId());
            json.insert("StoreType", categories[i].getStoreType());
            json.insert("SubCategoryId", categories[i].getSubCategoryId());

            QJsonDocument doc;

            doc.setObject(json);

            QByteArray request_body = doc.toJson();//转换数据

            //manager.post(request, request_body);

            //==========================================
            reply = manager.post(request, request_body);

            synchronous.exec();

            getPageCounts(reply, i);
            //==========================================
        }
    }

    getProducts();
}





void Spider::getProducts()
{

    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getProducts(QNetworkReply*)));

    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    request.setUrl(QUrl("http://www.ows.newegg.com/Search.egg/Query"));

    for(int i = 0; i < categories.size(); ++i)
    {
        if(categories[i].isCategory())
        {
            isSubCategory = false;

            emit manager.finished(reply);
        }
        else
        {
            for(int page_number = 0; page_number < categories[i].getPageCount(); ++page_number)
            {
                isSubCategory = true;

                QJsonObject json;

                //插入数据
                json.insert("NValue", categories[i].getNValue());
                json.insert("NodeId", categories[i].getNodeId());
                json.insert("StoreId", categories[i].getStoreId());
                json.insert("StoreType", categories[i].getStoreType());
                json.insert("PageNumber", page_number + 1);//插入页码信息
                json.insert("SubCategoryId", categories[i].getSubCategoryId());

                QJsonDocument doc;

                doc.setObject(json);

                QByteArray request_body = doc.toJson();//转换数据

                manager.post(request, request_body);
            }
        }
    }
}





void Spider::getMenus(QNetworkReply* reply)//当前函数调用一次
{
    disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMenus(QNetworkReply*)));//解绑信号关联

    getJsonDoc(reply, __FUNCTION__);

    reply->deleteLater();

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

        //DELETE UNUSED NODE IN MENUS & ROOTS

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

    reply->deleteLater();

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
                                             categories[i].toObject()["NValue"].toString(),//CHANGE INT TO STRING IN 02/18/14
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




//遗留问题：两个二级目录不能获得三级目录"DIY PC SuperCombo"&"Car Electronics"， 这是由于NEWEGG本身导致， 由此引出去重问题
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
                                                     categories[i].toObject()["NValue"].toString(),//CHANGE INT TO STRING IN 02/18/14
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

                reply->deleteLater();
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
        isCategory = true;

        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] IS NOT A CATEGORY";
    }

    if(--count)
    {
    }
    else
    {
        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getSubCategories(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

        getPageCounts();
    }
}





void Spider::getPageCounts(QNetworkReply* reply)
{
    static int count = categories.size();

    qDebug() << count;

    if(isSubCategory)
    {
        getJsonDoc(reply, __FUNCTION__);

        if(doc.isObject())
        {
            //JSON

            QJsonObject obj = doc.object();//临时对象

            QJsonArray array = obj["ProductGroups"].toArray();//临时对象

            QJsonObject page_info = array[0].toObject()["PageInfo"].toObject();//页面信息对象
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
    }
    else
    {
        isSubCategory = true;

        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] IS NOT A SUBCATEGORY";
    }

    if(--count)
    {
    }
    else
    {
        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getPageCounts(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

        //getProducts();
    }
}





void Spider::getProducts(QNetworkReply* reply)
{
    //使用lambda表达式获取三级目录数目
    static int count = [this]()->int
    {
        int count = 0;

        for(int i = 0; i < categories.size(); ++i)
        {
            if(categories[i].isCategory())
            {
                count = count + 1;
            }
            else
            {
                count = count + (int)categories[i].getPageCount();
            }
        }

        return count;
    }();

    qDebug() << count;

    if(isSubCategory)
    {
        getJsonDoc(reply, __FUNCTION__);

        reply->deleteLater();

        if(doc.isObject())
        {
            //JSON

            QJsonObject obj = doc.object();//临时对象

            QJsonArray array = obj["ProductGroups"].toArray();//临时对象

            QJsonArray products = array[0].toObject()["ProductDeals"].toArray();//产品信息数组

            for(int i = 0; i < products.size(); ++i)
            {
                this->products.append(Product(
                                          products[i].toObject()["NeweggItemNumber"].toString(),
                                          products[i].toObject()["Title"].toString(),
                                          products[i].toObject()["FinalPrice"].toString(),
                                          products[i].toObject()["OriginalPrice"].toString(),
                                          products[i].toObject()["PromotionText"].toString(),
                                          products[i].toObject()["ReviewSummary"].toObject()["TotalReviews"].toInt(),
                                          products[i].toObject()["Instock"].toBool()));

                //ADD FOR DEBUG IN 04/03/14
                //=================================================================================
//                qDebug() << "PRODUCT ID             : " << this->products[i].getProductId() << "\n"
//                         << "PRODUCT NAME           : " << this->products[i].getProductName() << "\n"
//                         << "PRODUCT FINAL PRICE    : " << this->products[i].getFinalPrice() << "\n"
//                         << "PRODUCt ORIGINAL PRICE : " << this->products[i].getOriginalPrice() << "\n"
//                         << "PRODUCT PROMOTION TEXT : " << this->products[i].getPromotionText() << "\n"
//                         << "PRODUCT REVIEWS        : " << this->products[i].getReviews() << "\n"
//                         << "PRODUCT IS IN STOCK    : " << this->products[i].isInStock() << "\n\n\n\n\n";
                //=================================================================================
              }
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
    }
    else
    {
        isSubCategory = true;

        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] IS NOT A SUBCATEGORY";
    }

    if(--count)
    {
    }
    else
    {
        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getProducts(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗  

        //ADD FOR CHECKOUT IN 03/01/14

        QFile file("D:/SPIDER");

        file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append);

        QTextStream out(&file);

        for(int i = 0; i < products.size(); ++i)
        {
            out << "PRODUCT ID             : " << products[i].getProductId() << "\n"
                << "PRODUCT NAME           : " << products[i].getProductName() << "\n"
                << "PRODUCT FINAL PRICE    : " << products[i].getFinalPrice() << "\n"
                << "PRODUCt ORIGINAL PRICE : " << products[i].getOriginalPrice() << "\n"
                << "PRODUCT PROMOTION TEXT : " << products[i].getPromotionText() << "\n"
                << "PRODUCT REVIEWS        : " << products[i].getReviews() << "\n"
                << "PRODUCT IS IN STOCK    : " << products[i].isInStock() << "\n\n\n\n\n";
        }
    }
}





void Spider::getJsonDoc(QNetworkReply* reply, QString FUNCTION)
{
    QByteArray response;

    if(reply->error() == QNetworkReply::NoError)//检查是否发生网络错误
    {
        response = reply->readAll();//获取响应信息同时清空

        doc = QJsonDocument::fromJson(response, &parse_error);//解析响应信息

        if(parse_error.error == QJsonParseError::NoError)//检查是否发生解析错误
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] SUCCESS";
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] PARSE ERROR"
                     << "ERROR CODE" << parse_error.error
                     << "ERROR STR" << parse_error.errorString();
        }
    }
    else
    {
        qDebug() << __TIME__ << "IN [" << FUNCTION << "] NETWORK ERROR"
                 << "ERROR CODE" << reply->error()
                 << "ERROR STR" << reply->errorString();

        qDebug() << response;
    }
}





void Spider::getPageCounts(QNetworkReply* reply, int index)
{
    //使用lambda表达式获取三级目录数目
    static int count = categories.size();

    qDebug() << count;

    if(isSubCategory)
    {
        getJsonDoc(reply, __FUNCTION__);

        reply->deleteLater();

        if(doc.isObject())
        {
            //JSON

            QJsonObject obj = doc.object();//临时对象

            QJsonArray array = obj["ProductGroups"].toArray();//临时对象

            QJsonObject page_info = array[0].toObject()["PageInfo"].toObject();//页面信息对象

            categories[index].setPageCount(page_info["PageCount"].toDouble());
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
    }
    else
    {
        isSubCategory = true;

        qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] IS NOT A SUBCATEGORY";
    }

    if(--count)
    {
    }
    else
    {
        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getPageCounts(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗
    }
}
