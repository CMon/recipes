#pragma once

#include <common/category.h>
#include <common/unit.h>
#include <common/util.h>

class Ingredient
{
public:
	Ingredient();
	Ingredient(const bool isLiquid, const bool withGluten, const bool withLactose);

	void setFoodCategory(const Category & category) { foodCategory = category; }
	Category getFoodCategory() const { return foodCategory; }

	bool getIsLiquid() const { return isLiquid; }
	bool getContainsGluten() const { return containsGluten; }
	bool getContainsLactose() const { return containsLactose; }

	void updateName(const QLocale & locale, const QString & name);
	Locale2String getNames() const { return name; }

	bool operator ==(const Ingredient & rhs) const;
	bool operator !=(const Ingredient & rhs) const;

	QString toString() const;

private:
	Locale2String name;
	Category foodCategory;
	bool isLiquid;
	bool containsGluten;
	bool containsLactose;
};

typedef QList<Ingredient> Ingredients;

class IngredientPOD
{
public:
	IngredientPOD(int c, const Unit & u, const Ingredient & i) : count(c), unit(u), ingredient(i) {}

	QString toString() const
	{
		return
				"Count: " + QString::number(count) + " "
				"Unit: "  + unit.toString() + " "
				"Ingredient: " + ingredient.toString();
	}

public:
	int count;
	Unit unit;
	Ingredient ingredient;
};

typedef QList<IngredientPOD> IngredientPODs;
