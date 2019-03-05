#include "locale2string.h"

#include <recipes/common/util.h>

#include <QHashIterator>
#include <QJsonObject>

bool Locale2String::operator ==(const Locale2String & rhs) const
{
	bool equal = mapping_.size() == rhs.mapping_.size();

	foreach(const QLocale & locale, mapping_.keys()) {
		equal = equal && (value(locale) == rhs.value(locale));
	}

	return equal;
}

bool Locale2String::operator !=(const Locale2String & rhs) const
{
	return !operator ==(rhs);
}

QString Locale2String::operator [](const QLocale & loc) const
{
	return mapping_[loc];
}

void Locale2String::add(const QLocale & loc, const QString & string)
{
	mapping_[loc] = string;
}

const QString Locale2String::value(const QString & lang) const
{
	return mapping_.value(lang);
}

const QString Locale2String::value(const QLocale & key) const
{
	return mapping_.value(key);
}

const QString Locale2String::value(const QLocale & key, const QString & defaultValue) const
{
	return mapping_.value(key, defaultValue);
}

int Locale2String::size() const
{
	return mapping_.size();
}

bool Locale2String::isEmpty() const
{
	return mapping_.isEmpty();
}

QString &Locale2String::operator [](const QLocale & loc)
{
	return mapping_[loc];
}

QString Locale2String::toString() const
{
	QStringList values;

	foreach(const QLocale & locale, mapping_.keys()) {
		values << locale.name() + "=" + value(locale);
	}

	return values.join(";");
}

QList<QLocale> Locale2String::keys() const
{
	return mapping_.keys();
}
