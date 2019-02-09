#pragma once

#include <rpclib/common/jsonserialize.h>

#include <recipes/common/locale2string.h>

class Category : public JsonSerialize
{
	Q_OBJECT
	Q_PROPERTY(Locale2String name READ getNames WRITE updateName)
	Q_PROPERTY(bool isFoodCategory READ isFoodCategory)

public:
	Category(bool isFoodCategory = false) : isFoodCategory_(isFoodCategory) {}

	bool isFoodCategory() const { return isFoodCategory_; }

	void updateName(const QLocale & locale, const QString & name_);
	Locale2String getNames() const { return name_; }

	bool operator ==(const Category & rhs) const;
	bool operator !=(const Category & rhs) const;

	QString toString() const;

private:
	Locale2String name_;
	bool isFoodCategory_;
};

typedef QList<Category> Categories;
