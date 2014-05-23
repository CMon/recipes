#pragma once

#include <common/util.h>

class Unit
{
public:
	Unit(int g, int m);

	void setCount(int c) { count = c; }
	int getCount() { return count; }

private:
	Locale2String abbreviation;
	Locale2String fullName;
	int gram;
	int ml;

	int count;
};
