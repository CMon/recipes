#pragma once

#include <rpclib/common/jsonserialize.h>

#include <recipes/common/locale2string.h>
#include <recipes/common/category.h>
#include <recipes/common/unit.h>
#include <recipes/common/util.h>

class Ingredient : public JsonSerialize
{
	Q_OBJECT
	Q_PROPERTY(Locale2String name READ getNames WRITE updateName)
	Q_PROPERTY(Category foodCategory READ getFoodCategory WRITE setFoodCategory)
	Q_PROPERTY(bool isLiquid READ getIsLiquid)
	Q_PROPERTY(bool containsGluten READ getContainsGluten)
	Q_PROPERTY(bool containsLactose READ getContainsLactose)

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

private:
	Locale2String name_;
	Category foodCategory_;
	bool isLiquid_;
	bool containsGluten_;
	bool containsLactose_;
};

typedef QList<Ingredient> Ingredients;

class IngredientPOD : public JsonSerialize
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

public serialized:
	int count;
	Unit unit;
	Ingredient ingredient;
	bool isOptional;
};

typedef QList<IngredientPOD> IngredientPODs;
