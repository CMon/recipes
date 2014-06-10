#pragma once

#include <common/locale2string.h>

class Portion
{
public:
	Portion();

	void updateDescriptions(const QLocale & locale, const QString & description);
	Locale2String getDescriptions() const { return description; }

	void setCount(int count) { this->count = count; }
	int getCount() const { return count; }

	bool operator ==(const Portion & rhs) const;
	bool operator !=(const Portion & rhs) const;

	QString toString() const;

private:
	int count;
	Locale2String description;
};
