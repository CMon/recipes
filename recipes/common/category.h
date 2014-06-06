#pragma once

#include <common/util.h>

class Category
{
public:
	Category(bool isFoodCategory = false) : isFoodCategory_(isFoodCategory) {}

	bool isFoodCategory() const { return isFoodCategory_; }

	void updateName(const QLocale & locale, const QString & name);
	Locale2String getNames() const { return name; }

	bool operator ==(const Category & rhs) const;
	bool operator !=(const Category & rhs) const;

	QString toString() const;

private:
	Locale2String name;
	bool isFoodCategory_;
};

typedef QList<Category> Categories;
