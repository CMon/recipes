#pragma once

#include <common/category.h>
#include <common/unit.h>
#include <common/util.h>

class Ingredient
{
public:
	Ingredient(const Locale2String & name, const Unit & unit, const Category & category);

	void setIsOptional(bool optional) { isOptional = optional; }
	bool getIsOptional() { return isOptional; }

private:
	Locale2String name;
	Unit unit;
	Category foodCategory;
	bool isLiquid;
	bool containsGluten;
	bool containsLactose;

	bool isOptional;
};

typedef QList<Ingredient> Ingredients;
