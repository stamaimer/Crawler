#include "spider.h"

Spider::Spider(QWidget* parent) : QWidget(parent)
{
    timer.start();//启动计时

    tree = new QTreeWidget(this);//创建树形控件

    console = new QTextEdit(this);

    console->setReadOnly(true);

    //qInstallMessageHandler(redirect);

    QVBoxLayout *layout = new QVBoxLayout();//创建垂直布局

    layout->addWidget(tree);//添加树形控件
    layout->addWidget(console);

    this->setLayout(layout);//设置布局

    initDatabase();

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
    this->menus.clear();

    this->menus.append(6);
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

    //getProducts();

    for(int i = 0; i < categories.size(); ++i)
    {
        if(categories[i].isCategory())
        {
        }
        else
        {
            sub_categories.append(categories[i]);
        }
    }

    contract();

    for(int i = 0; i < packets.size(); ++i)
    {
        qDebug() << packets[i].getDescription()
                 << (int)packets[i].getPageCount()
                 << packets[i].getBegin()
                 << packets[i].getEnd();
    }

    //开启消费者线程
    //==================================
//    for(int i = 0; i < 1; ++i)
//    {
//        cthreads[i] = new CThread(this);
//        cthreads[i]->start(i + 1);
//    }
    //==================================

    //开启生产者线程
    //==================================
    for(int i = 0; i < 100; ++i)
    {
        pthreads[i] = new PThread(this);
        pthreads[i]->start(i + 1);
    }
    //==================================

    for(int i = 0; i < 100; ++i)
    {
        pthreads[i]->wait();
    }

    for(int i = 0; i < 100; ++i)
    {
        pthreads[i]->deleteLater();
    }
}





//当期函数功能移至生产者线程
//void Spider::getProducts()
//{

//    //connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getProducts(QNetworkReply*)));

//    connect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(addReply(QNetworkReply*)));

//    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

//    request.setUrl(QUrl("http://www.ows.newegg.com/Search.egg/Query"));

//    for(int i = 0; i < categories.size(); ++i)
//    {
//        if(categories[i].isCategory())
//        {
//            isSubCategory = false;

//            emit manager.finished(reply);
//        }
//        else
//        {
//            for(int page_number = 0; page_number < (int)categories[i].getPageCount(); ++page_number)
//            {
//                isSubCategory = true;

//                QJsonObject json;

//                //插入数据
//                json.insert("NValue", categories[i].getNValue());
//                json.insert("NodeId", categories[i].getNodeId());
//                json.insert("StoreId", categories[i].getStoreId());
//                json.insert("StoreType", categories[i].getStoreType());
//                json.insert("PageNumber", page_number + 1);//插入页码信息
//                json.insert("SubCategoryId", categories[i].getSubCategoryId());

//                QJsonDocument doc;

//                doc.setObject(json);

//                QByteArray request_body = doc.toJson();//转换数据

//                manager.post(request, request_body);
//            }
//        }
//    }
//}





void Spider::getMenus(QNetworkReply* reply)//当前函数调用一次
{
    disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getMenus(QNetworkReply*)));//解绑信号关联

    if(getJsonDoc(reply, __FUNCTION__))
    {
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

            //DELETE UNUSED NODE IN MENUS & ROOTS。。。

            tree->addTopLevelItems(roots);//添加顶级目录结点

            qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

            getCategories();
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
    }
}





void Spider::getCategories(QNetworkReply* reply)
{
    static int count = menus.size();//获得当前函数运行次数

    qDebug() << count;

    if(getJsonDoc(reply, __FUNCTION__))
    {
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

                leaves[i]->setCheckState(0, Qt::Unchecked);//设置选择状态

                this->leaves.append(leaf);
            }

            int index = menus.indexOf(QUrlQuery(reply->request().url()).queryItemValue("storeId").toInt());//获取索引

            roots[index]->addChildren(leaves);//每个顶级目录添加次级目录
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
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
        if(getJsonDoc(reply, __FUNCTION__))
        {
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
        if(getJsonDoc(reply, __FUNCTION__))
        {
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





void Spider::getProducts(QNetworkReply* reply, Packet packet)
{
    //使用lambda表达式获取三级目录数目
    static int count = [this]()->int
    {
        int count = 0;

        for(int i = 0; i < categories.size(); ++i)
        {
            if(categories[i].isCategory())
            {
                //count = count + 1;
            }
            else
            {
                count = count + (int)categories[i].getPageCount();
            }
        }

        return count;
    }();

    qDebug() << count;

    if(getJsonDoc(reply, packet, __FUNCTION__))
    {
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
            }
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << __FUNCTION__ << "] DATA ERROR";
        }
    }

    if(--count)
    {
    }
    else
    {
//        disconnect(&manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(getProducts(QNetworkReply*)));//解绑信号关联

        qDebug() << "TIME ELAPSED" << timer.elapsed() / 1000;//输出时间消耗

        //ADD FOR CHECKOUT IN 03/01/14

        QFile file("d:/spider.log");

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

        qDebug() << "END OF FILE OUTPUT";
    }
}




int Spider::getJsonDoc(QNetworkReply* reply, QString FUNCTION)
{
    QByteArray response;

    if(reply->error() == QNetworkReply::NoError)//检查是否发生网络错误
    {
        response = reply->readAll();//获取响应信息同时清空

        doc = QJsonDocument::fromJson(response, &parse_error);//解析响应信息

        if(parse_error.error == QJsonParseError::NoError)//检查是否发生解析错误
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] SUCCESS";

            return 1;
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] PARSE ERROR"
                     << "ERROR CODE" << parse_error.error
                     << "ERROR STR" << parse_error.errorString();

            return 0;
        }
    }
    else
    {
        qDebug() << __TIME__ << "IN [" << FUNCTION << "] NETWORK ERROR"
                 << "ERROR CODE" << reply->error()
                 << "ERROR STR" << reply->errorString();

        return 0;
    }
}



int Spider::getJsonDoc(QNetworkReply* reply, Packet packet, QString FUNCTION)
{
    QByteArray response;

    if(reply->error() == QNetworkReply::NoError)//检查是否发生网络错误
    {
        response = reply->readAll();//获取响应信息同时清空

        doc = QJsonDocument::fromJson(response, &parse_error);//解析响应信息

        if(parse_error.error == QJsonParseError::NoError)//检查是否发生解析错误
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] SUCCESS";

            return 1;
        }
        else
        {
            qDebug() << __TIME__ << "IN [" << FUNCTION << "] PARSE ERROR"
                     << "ERROR CODE" << parse_error.error
                     << "ERROR STR" << parse_error.errorString();

            return 0;
        }
    }
    else
    {
        qDebug() << __TIME__ << "IN [" << FUNCTION << "] NETWORK ERROR"
                 << "ERROR CODE" << reply->error()
                 << "ERROR STR" << reply->errorString();

        packets.append(packet);

        qDebug() << packet.getDescription() << packet.getBegin();

        return 0;
    }
}





void Spider::initDatabase()
{
    database = QSqlDatabase::addDatabase("QMYSQL");

    database.setHostName("localhost");

    database.setUserName("root");

    database.setPassword("");

    database.setDatabaseName("spider");

    if(!database.open())
    {
        qDebug() << database.lastError();
    }
}





void Spider::getPageCounts(QNetworkReply* reply, int index)
{
    static int count = categories.size();

    qDebug() << count;

    if(isSubCategory)
    {
        if(getJsonDoc(reply, __FUNCTION__))
        {
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





//void Spider::addReply(QNetworkReply* reply)
//{
//    qDebug() << "in slot";

//    mutex.lock();

//    replys.append(reply);

//    mutex.unlock();

//    qDebug() << "leave slot";
//}






void Spider::contract()
{

    for(int i = 0; i < sub_categories.size(); ++i)
    {
        int page_count = (int)sub_categories[i].getPageCount();

        for(int packet_number = 0; packet_number < page_count; ++packet_number)
        {
            Packet packet = Packet(sub_categories[i].getNodeId(),
                                   sub_categories[i].getStoreId(),
                                   sub_categories[i].getStoreType(),
                                   sub_categories[i].getCategoryId(),
                                   sub_categories[i].getSubCategoryId(),
                                   sub_categories[i].getNValue(),
                                   sub_categories[i].getPageCount(),
                                   sub_categories[i].isCategory(),
                                   sub_categories[i].getDescription());

            packet.setBegin(packet_number);

            if(packet_number == page_count)
            {
                packet.setEnd(page_count);
            }
            else
            {
                packet.setEnd(packet_number + 1);
            }

            packets.append(packet);
        }
    }
}





void Spider::redirect(QtMsgType type, const QMessageLogContext& context, const QString& msg)
{
    console->append(msg);
}



















