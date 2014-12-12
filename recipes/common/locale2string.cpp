#include "locale2string.h"

#include <recipes/common/util.h>

bool Locale2String::operator ==(const Locale2String & rhs) const
{
	bool equal = size() == rhs.size();

	foreach(const QLocale & locale, keys()) {
		equal = equal && (value(locale) == rhs.value(locale));
	}

	return equal;
}

bool Locale2String::operator !=(const Locale2String & rhs) const
{
	return !operator ==(rhs);
}

QString Locale2String::toString() const
{
	QStringList values;

	foreach(const QLocale & locale, keys()) {
		values << locale.name() + "=" + value(locale);
	}

	return values.join(";");
}
