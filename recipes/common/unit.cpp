#include "unit.h"

Unit::Unit(int g, int m)
    : gram(g),
      ml(m),
      count(0)
{
}

bool Unit::isNull() const
{
	return gram == -1 && ml == -1;
}

void Unit::updateAbbreviation(const QLocale & locale, const QString & abbrev)
{
	abbreviation[locale] = abbrev;
}

void Unit::updateCompleteName(const QLocale & locale, const QString & name)
{
	completeName[locale] = name;
}

bool Unit::operator ==(const Unit & rhs) const
{
	bool different =
	        gram  == rhs.gram &&
	        ml    == rhs.ml &&
	        count == rhs.count &&
	        abbreviation.size() == rhs.abbreviation.size() &&
	        completeName.size() == rhs.completeName.size();

	foreach (const QLocale & locale, abbreviation) {
		different = different && (rhs.abbreviation[locale] == abbreviation[locale]);
	}

	foreach (const QLocale & locale, completeName) {
		different = different && (rhs.completeName[locale] == completeName[locale]);
	}

	return different;
}

bool Unit::operator !=(const Unit & rhs) const
{
	return !operator ==(rhs);
}

QString Unit::toString() const
{
	QString retval =
	        "inGram: " + QString::number(gram) +
	        " inMl: "   + QString::number(ml);
	retval += " abbreviations: [";
	foreach(const QLocale & locale, abbreviation.keys()) {
		retval += locale.name() + "=" + abbreviation.value(locale);
	}
	retval += "] completeNames: [";
	foreach(const QLocale & locale, completeName.keys()) {
		retval += locale.name() + "=" + completeName.value(locale);
	}
	retval += "]";

	return retval;
}
