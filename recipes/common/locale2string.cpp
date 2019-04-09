#include "locale2string.h"

Locale2String::Locale2String()
{
}

Locale2String::Locale2String(const QString & loc, const QString & value)
{
	mapping_[QLocale(loc)] = value;
}

Locale2String::Locale2String(const QLocale & loc, const QString & value)
{
	mapping_[loc] = value;
}

bool Locale2String::operator ==(const Locale2String & rhs) const
{
	if (mapping_.size() != rhs.mapping_.size()) return false;

	foreach(const QLocale & locale, mapping_.keys()) {
		if (value(locale) != rhs.value(locale)) return false;
	}

	return true;
}

bool Locale2String::operator !=(const Locale2String & rhs) const
{
	return !operator ==(rhs);
}

QString Locale2String::operator [](const QLocale & loc) const
{
	return mapping_[loc];
}

void Locale2String::add(const Locale2String & loc2s)
{
	for (const QLocale & key: loc2s.keys()) {
		mapping_[key] = loc2s.value(key);
	}
}

void Locale2String::add(const QLocale & loc, const QString & string)
{
	mapping_[loc] = string;
}

bool Locale2String::contains(const QLocale & loc)
{
	return mapping_.contains(loc);
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
