#pragma once

#include <common/util.h>

class Category
{
public:
	Category(const Locale2String & name);

private:
	Locale2String name;
	bool isFoodCategory;
};

typedef QList<Category> Categories;
