#include "unit.h"

Unit::Unit(int g, int m)
    : gram(g),
      ml(m)
{
}

void Unit::updateAbbreviation(const QLocale & locale, const QString & abbrev)
{
	abbreviation[locale] = abbrev;
}

void Unit::updateCompleteName(const QLocale & locale, const QString & name)
{
	completeName[locale] = name;
}
