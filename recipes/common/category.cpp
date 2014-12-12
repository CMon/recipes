#include "category.h"

#include <recipes/common/util.h>

void Category::updateName(const QLocale & locale, const QString & name)
{
	this->name[locale] = name;
}

bool Category::operator ==(const Category & rhs) const
{
	return
	        isFoodCategory_ == rhs.isFoodCategory_ &&
	        name == rhs.name;
}

bool Category::operator !=(const Category & rhs) const
{
	return !operator ==(rhs);
}

QString Category::toString() const
{
	return
	        "isFoodCategory: " + Util::boolToString(isFoodCategory_) + " "
	        "names: ["         + name.toString() + " ";
}
