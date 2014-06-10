#pragma once

#include <common/locale2string.h>

class Portion
{
public:
	Portion();

	void updateDescriptions(const QLocale & locale, const QString & description);
	Locale2String getDescriptions() const { return description; }

	bool operator ==(const Portion & rhs) const;
	bool operator !=(const Portion & rhs) const;

	QString toString() const;

private:
	Locale2String description;
};
