#pragma once

#include <QLocale>
#include <QMetaType>

class Locale2String : public QHash<QLocale, QString>
{
public:
	QJsonValue toJson() const;
	static Locale2String fromJson(const QJsonValue & value);

public:
	bool operator ==(const Locale2String & rhs) const;
	bool operator !=(const Locale2String & rhs) const;

	QString toString() const;
};
Q_DECLARE_METATYPE(Locale2String);
