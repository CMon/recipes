#pragma once

#include <recipes/common/locale2string.h>

#include <cereal/cereal.hpp>

class Unit
{
public:
	Unit() : gram(-1), ml(-1) {}
	Unit(int g, int m);

	bool isNull() const;

	void setCount(int c) { count = c; }
	int getCount() const { return count; }

	void updateAbbreviation(const QLocale & locale, const QString & abbrev);
	Locale2String getAbbreviations() const { return abbreviation; }

	void updateCompleteName(const QLocale & locale, const QString & name);
	Locale2String getCompleteNames() const { return completeName; }

	int getGram() const { return gram; }
	int getMl() const { return ml; }

	bool operator ==(const Unit & rhs) const;
	bool operator !=(const Unit & rhs) const;

	QString toString() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("abbreviation", abbreviation),
		   cereal::make_nvp("completeName", completeName),
		   cereal::make_nvp("gram", gram),
		   cereal::make_nvp("ml", ml),
		   cereal::make_nvp("count", count));
	}

private:
	Locale2String abbreviation;
	Locale2String completeName;
	int gram;
	int ml;

	int count;
};
