#ifndef PRODUCT_H
#define PRODUCT_H

#include <QString>

class Product
{
    QString product_id;
    QString product_name;

    QString final_price;
    QString original_price;

    QString promotion_text;

    int reviews;

    bool is_in_stock;

public:
    Product(){}

    Product(QString product_id,
            QString product_name,
            QString final_price,
            QString original_price,
            QString promotion_text,
            int reviews,
            bool is_in_stock)
    {
        this->product_id = product_id;
        this->product_name = product_name;

        this->final_price = final_price;
        this->original_price = original_price;

        this->promotion_text = promotion_text;

        this->reviews = reviews;

        this->is_in_stock = is_in_stock;
    }

    QString getProductId()
    {
        return product_id;
    }

    QString getProductName()
    {
        return product_name;
    }

    QString getFinalPrice()
    {
        return final_price;
    }

    QString getOriginalPrice()
    {
        return original_price;
    }

    QString getPromotionText()
    {
        return promotion_text;
    }

    int getReviews()
    {
        return reviews;
    }

    bool isInStock()
    {
        return is_in_stock;
    }
};

#endif // PRODUCT_H
