#include "locale2string.h"

#include <recipes/common/util.h>

#include <QHashIterator>
#include <QJsonObject>

QJsonValue Locale2String::toJson() const
{
	QJsonObject retval;
	QHashIterator<QLocale, QString> iter(*this);
	while (iter.hasNext()) {
		iter.next();
		retval.insert(iter.key().bcp47Name(), iter.value());
	}
	return retval;
}

Locale2String Locale2String::fromJson(const QJsonValue & value)
{
	Locale2String retval;
	QJsonObject hash = value.toObject();
	for(const QString & key: hash.keys()) {
		retval[key] = hash.value(key).toString();
	}
	return retval;
}

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
