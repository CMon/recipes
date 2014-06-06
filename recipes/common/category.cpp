#include "category.h"

void Category::updateName(const QLocale & locale, const QString & name)
{
	this->name[locale] = name;
}

bool Category::operator ==(const Category & rhs) const
{
	bool different = isFoodCategory_ == rhs.isFoodCategory_ &&
	                 name.size()     == rhs.name.size();

	foreach (const QLocale & locale, name) {
		different = different && (rhs.name[locale] == name[locale]);
	}

	return different;
}

bool Category::operator !=(const Category & rhs) const
{
	return !operator ==(rhs);
}

QString Category::toString() const
{
	QString retval =
	        "isFoodCategory: " + Util::boolToString(isFoodCategory_);
	retval += " names: [";
	foreach(const QLocale & locale, name.keys()) {
		retval += locale.name() + "=" + name.value(locale) + " ";
	}

	return retval;
}
