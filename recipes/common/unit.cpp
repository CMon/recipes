#include "unit.h"

#include <common/util.h>

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
	return
	        gram  == rhs.gram &&
	        ml    == rhs.ml &&
	        count == rhs.count &&
	        abbreviation == rhs.abbreviation &&
	        completeName == rhs.completeName;
}

bool Unit::operator !=(const Unit & rhs) const
{
	return !operator ==(rhs);
}

QString Unit::toString() const
{
	return
			"inGram: " + QString::number(gram) + " "
			"inMl: "   + QString::number(ml)   + " "
			"abbreviations: [" + abbreviation.toString() + "] "
			"completeNames: [" + completeName.toString() + "] ";
}
