#include "portion.h"

#include <common/util.h>

Portion::Portion()
    : count(0)
{
}

bool Portion::isNull() const
{
	return
	        count == 0 &&
	        description.isEmpty();
}

void Portion::updateDescriptions(const QLocale & locale, const QString & description)
{
	this->description[locale] = description;
}

bool Portion::operator ==(const Portion & rhs) const
{
	return
	        count == rhs.count &&
	        description == rhs.description;
}

bool Portion::operator !=(const Portion & rhs) const
{
	return !operator ==(rhs);
}

QString Portion::toString() const
{
	return
	        "count: "  + QString::number(count) + " "
	        "names: [" + description.toString() + "]";
}
