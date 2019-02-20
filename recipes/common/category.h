#pragma once

#include <recipes/common/locale2string.h>

#include <cereal/cereal.hpp>
#include <rpclib/common/types/types.h>

class Category
{
public:
	Category(bool isFoodCategory = false) : isFoodCategory_(isFoodCategory) {}

	bool isFoodCategory() const { return isFoodCategory_; }

	void updateName(const QLocale & locale, const QString & name_);
	Locale2String getNames() const { return name_; }

	bool operator ==(const Category & rhs) const;
	bool operator !=(const Category & rhs) const;

	QString toString() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("name", name_),
		   cereal::make_nvp("isFoodCategory", isFoodCategory_));
	}

private:
	Locale2String name_;
	bool isFoodCategory_;
};
CEREAL_CLASS_VERSION(Category, 1);

typedef QList<Category> Categories;
