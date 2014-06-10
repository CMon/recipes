#pragma once

#include <QLocale>

class Locale2String : public QHash<QLocale, QString>
{
public:
	bool operator ==(const Locale2String & rhs) const;
	bool operator !=(const Locale2String & rhs) const;

	QString toString() const;
};
