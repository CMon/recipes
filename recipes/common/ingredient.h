#pragma once

#include <recipes/common/locale2string.h>
#include <recipes/common/category.h>
#include <recipes/common/unit.h>
#include <recipes/common/util.h>

#include <rpclib/common/types/types.h>

class Ingredient
{
public:
	Ingredient();
	Ingredient(const bool isLiquid_, const bool withGluten, const bool withLactose);

	void setFoodCategory(const Category & category) { foodCategory_ = category; }
	Category getFoodCategory() const { return foodCategory_; }

	bool getIsLiquid() const { return isLiquid_; }
	bool getContainsGluten() const { return containsGluten_; }
	bool getContainsLactose() const { return containsLactose_; }

	void updateName(const QLocale & locale, const QString & name_);
	Locale2String getNames() const { return name_; }

	bool operator ==(const Ingredient & rhs) const;
	bool operator !=(const Ingredient & rhs) const;

	QString toString() const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("name", name_),
		   cereal::make_nvp("foodCategory", foodCategory_),
		   cereal::make_nvp("isLiquid", isLiquid_),
		   cereal::make_nvp("containsGluten", containsGluten_),
		   cereal::make_nvp("containsLactose", containsLactose_)
		   );
	}

private:
	Locale2String name_;
	Category foodCategory_;
	bool isLiquid_;
	bool containsGluten_;
	bool containsLactose_;
};
CEREAL_CLASS_VERSION(Ingredient, 1);

typedef QList<Ingredient> Ingredients;

class IngredientPOD
{
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

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("count", count),
		   cereal::make_nvp("unit", unit),
		   cereal::make_nvp("ingredient", ingredient),
		   cereal::make_nvp("isOptional", isOptional));
	}

public:
	int count;
	Unit unit;
	Ingredient ingredient;
	bool isOptional;
};
CEREAL_CLASS_VERSION(IngredientPOD, 1);

typedef QList<IngredientPOD> IngredientPODs;
