#pragma once

#include <common/util.h>

class Unit
{
public:
	Unit(int g, int m);

	void setCount(int c) { count = c; }
	int getCount() const { return count; }

	void updateAbbreviation(const QLocale & locale, const QString & abbrev);
	Locale2String getAbbreviations() const { return abbreviation; }

	void updateCompleteName(const QLocale & locale, const QString & name);
	Locale2String getCompleteNames() const { return completeName; }

	int getGram() const { return gram; }
	int getMl() const { return ml; }

private:
	Locale2String abbreviation;
	Locale2String completeName;
	int gram;
	int ml;

	int count;
};
