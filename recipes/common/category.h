#pragma once

#include <common/util.h>

class Category
{
public:
    Category();

private:
    Locale2String name;
    bool isFoodCategory;
};

typedef QList<Category> Categories;
