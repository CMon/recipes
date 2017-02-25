#pragma once

#include <recipes/common/locale2string.h>

#include <cflib/serialize/serialize.h>

class Portion
{
	SERIALIZE_CLASS

public:
	Portion();

	bool isNull() const;

	void updateDescriptions(const QLocale & locale, const QString & description);
	Locale2String getDescriptions() const { return description; }

	void setCount(int count) { this->count = count; }
	int getCount() const { return count; }

	bool operator ==(const Portion & rhs) const;
	bool operator !=(const Portion & rhs) const;

	QString toString() const;

private serialized:
	int count;
	Locale2String description;
};
