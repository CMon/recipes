#pragma once

#include <rpclib/common/types/types.h>
#include <cereal/cereal.hpp>
#include <QLocale>

class Locale2String
{
public:
	Locale2String();
	Locale2String(const QString & loc, const QString & value);
	Locale2String(const QLocale & loc, const QString & value);

	bool operator ==(const Locale2String & rhs) const;
	bool operator !=(const Locale2String & rhs) const;

	QString & operator [](const QLocale & loc);
	QString operator [](const QLocale & loc) const;
	void add(const Locale2String & loc2s);
	void add(const QLocale &loc, const QString & string);
	bool contains(const QLocale & loc);

	const QString value(const QString &lang) const;
	const QString value(const QLocale &key) const;
	const QString value(const QLocale &key, const QString & defaultValue) const;

	int size() const;
	bool isEmpty() const;

	QString toString() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("languageMapping", mapping_));
	}

	QList<QLocale> keys() const;

	inline QHash<QLocale, QString>::iterator begin() { return mapping_.begin(); }
	inline QHash<QLocale, QString>::iterator end() { return mapping_.end(); }

	inline QHash<QLocale, QString>::const_iterator begin() const { return mapping_.begin(); }
	inline QHash<QLocale, QString>::const_iterator end() const { return mapping_.end(); }

private:
	QHash<QLocale, QString> mapping_;
};
Q_DECLARE_METATYPE(Locale2String);
CEREAL_CLASS_VERSION(Locale2String, 1);
