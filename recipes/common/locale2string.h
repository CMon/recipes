#pragma once

#include <QLocale>

#include <cflib/serialize/serialize.h>

class Locale2String : public QHash<QLocale, QString>
{
	SERIALIZE_CLASS

public:
	bool operator ==(const Locale2String & rhs) const;
	bool operator !=(const Locale2String & rhs) const;

	QString toString() const;
};
