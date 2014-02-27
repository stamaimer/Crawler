#ifndef CATEGORY_H
#define CATEGORY_H

#include <QString>

class Category
{
    int node_id;

    int store_id;
    int store_type;

    int category_id;
    int sub_category_id;

    int n_value;

    int page_count;

    bool is_category;

    //ADD FOR DEBUG IN 02/27/14
    QString description;

public:
    Category(){}

    Category(int node_id,
             int store_id,
             int store_type,
             int category_id,
             int sub_category_id,
             int n_value,
             int page_count,
             bool is_category,
             QString description)
    {
        this->node_id = node_id;

        this->store_id = store_id;
        this->store_type = store_type;

        this->category_id = category_id;
        this->sub_category_id = sub_category_id;

        this->n_value = n_value;

        this->page_count = page_count;

        this->is_category = is_category;

        this->description = description;
    }

    int getNodeId()
    {
        return node_id;
    }

    int getStoreId()
    {
        return store_id;
    }

    int getStoreType()
    {
        return store_type;
    }

    int getCategoryId()
    {
        return category_id;
    }

    int getSubCategoryId()
    {
        return sub_category_id;
    }

    int getNValue()
    {
        return n_value;
    }

    int getPageCount()
    {
        return page_count;
    }

    void setPageCount(int page_count)
    {
        this->page_count = page_count;
    }

    bool isCategory()
    {
        return is_category;
    }

    //ADD FOR DEBUG IN 02/27/14
    QString getDescription()
    {
        return description;
    }
};

#endif // CATEGORY_H
