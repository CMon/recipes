#pragma once

#include <recipes/common/locale2string.h>
#include <recipes/common/category.h>
#include <recipes/common/unit.h>
#include <recipes/common/util.h>

#include <cflib/serialize/serialize.h>

class Ingredient
{
	SERIALIZE_CLASS

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

private serialized:
	Locale2String name;
	Category foodCategory;
	bool isLiquid;
	bool containsGluten;
	bool containsLactose;
};

typedef QList<Ingredient> Ingredients;

class IngredientPOD
{
	SERIALIZE_CLASS

public:
	IngredientPOD()
	    : count(0), isOptional(false) {}
	IngredientPOD(int c, const Unit & u, const Ingredient & i, const bool isOptional = false)
	    : count(c), unit(u), ingredient(i), isOptional(isOptional) {}

	bool operator ==(const IngredientPOD & rhs) const {
		return
		        count      == rhs.count &&
		        unit       == rhs.unit &&
		        ingredient == rhs.ingredient &&
		        isOptional == rhs.isOptional;
	}

	bool operator !=(const IngredientPOD & rhs) const {
		return !operator ==(rhs);
	}

	QString toString() const
	{
		return
		        "Count: " + QString::number(count) + " "
		        "Unit: "  + unit.toString() + " "
		        "Ingredient: " + ingredient.toString() + " "
		        "isOptional: " + Util::boolToString(isOptional)
		        ;
	}

public serialized:
	int count;
	Unit unit;
	Ingredient ingredient;
	bool isOptional;
};

typedef QList<IngredientPOD> IngredientPODs;
