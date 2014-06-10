#include "portion.h"

#include <common/util.h>

Portion::Portion()
{
}

void Portion::updateDescriptions(const QLocale & locale, const QString & description)
{
	this->description[locale] = description;
}

bool Portion::operator ==(const Portion & rhs) const
{
	return description == rhs.description;
}

bool Portion::operator !=(const Portion & rhs) const
{
	return !operator ==(rhs);
}

QString Portion::toString() const
{
	return
	        "names: [" + description.toString() + "]";
}
