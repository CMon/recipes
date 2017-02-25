#pragma once

#include <recipes/common/locale2string.h>

#include <cflib/serialize/serialize.h>

class Category
{
	SERIALIZE_CLASS

public:
	Category(bool isFoodCategory = false) : isFoodCategory_(isFoodCategory) {}

	bool isFoodCategory() const { return isFoodCategory_; }

	void updateName(const QLocale & locale, const QString & name);
	Locale2String getNames() const { return name; }

	bool operator ==(const Category & rhs) const;
	bool operator !=(const Category & rhs) const;

	QString toString() const;

private serialized:
	Locale2String name;
	bool isFoodCategory_;
};

typedef QList<Category> Categories;
