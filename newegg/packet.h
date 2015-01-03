#ifndef PACKET_H
#define PACKET_H

#include "category.h"

class Packet
{
    int node_id;

    int store_id;
    int store_type;

    int category_id;
    int sub_category_id;

    QString n_value;

    double page_count;

    bool is_category;

    QString description;

    int begin;
    int end;

public:
    Packet(){}

    Packet(int node_id,
           int store_id,
           int store_type,
           int category_id,
           int sub_category_id,
           QString n_value,
           double page_count,
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

    QString getNValue()
    {
        return n_value;
    }

    double getPageCount()
    {
        return page_count;
    }

    void setPageCount(double page_count)
    {
        this->page_count = page_count;
    }

    bool isCategory()
    {
        return is_category;
    }

    QString getDescription()
    {
        return description;
    }

    void setBegin(int begin)
    {
        this->begin = begin;
    }

    int getBegin()
    {
        return begin;
    }

    void setEnd(int end)
    {
        this->end = end;
    }

    int getEnd()
    {
        return end;
    }
};

#endif // PACKET_H
