#include "category.h"

#include <recipes/common/util.h>

void Category::updateName(const QLocale & locale, const QString & name)
{
	name_[locale] = name;
}

bool Category::operator ==(const Category & rhs) const
{
	return
	        isFoodCategory_ == rhs.isFoodCategory_ &&
	        name_           == rhs.name_;
}

bool Category::operator !=(const Category & rhs) const
{
	return !operator ==(rhs);
}

QString Category::toString() const
{
	return QString("isFoodCategory: %1 names: [%2]")
	        .arg(Util::boolToString(isFoodCategory_))
	        .arg(name_.toString());
}
