#include "portion.h"

Portion::Portion()
{
}

void Portion::updateDescriptions(const QLocale & locale, const QString & description)
{
	this->description[locale] = description;
}

bool Portion::operator ==(const Portion & rhs) const
{
	bool equal = description.size()     == rhs.description.size();
	foreach (const QLocale & locale, description) {
		equal = equal && (rhs.description[locale] == description[locale]);
	}

	return equal;
}

bool Portion::operator !=(const Portion & rhs) const
{
	return !operator ==(rhs);
}

QString Portion::toString() const
{
	QString retval =
	        "names: [";
	foreach(const QLocale & locale, description.keys()) {
		retval += locale.name() + "=" + description.value(locale) + " ";
	}
	retval += "]";

	return retval;
}
