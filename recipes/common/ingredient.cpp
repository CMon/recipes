#include "ingredient.h"

#include <recipes/common/util.h>

Ingredient::Ingredient()
    : isLiquid_(false),
      containsGluten_(false),
      containsLactose_(false)
{
}

Ingredient::Ingredient(const bool isLiquid, const bool withGluten, const bool withLactose)
    :
      isLiquid_(isLiquid),
      containsGluten_(withGluten),
      containsLactose_(withLactose)
{
}

void Ingredient::updateName(const QLocale & locale, const QString & name)
{
	this->name_[locale] = name;
}

bool Ingredient::operator ==(const Ingredient & rhs) const
{
	return
	        foodCategory_    == rhs.foodCategory_ &&
	        isLiquid_        == rhs.isLiquid_ &&
	        containsGluten_  == rhs.containsGluten_ &&
	        containsLactose_ == rhs.containsLactose_ &&
	        name_            == rhs.name_;
}

bool Ingredient::operator !=(const Ingredient & rhs) const
{
	return !operator ==(rhs);
}

QString Ingredient::toString() const
{
	return
	        "foodCategory: "    + foodCategory_.toString()             + "; "
	        "isLiquid: "        + Util::boolToString(isLiquid_)        + "; "
	        "containsGluten: "  + Util::boolToString(containsGluten_)  + "; "
	        "containsLactose: " + Util::boolToString(containsLactose_) + "; "
	        "names: ["          + name_.toString()                     + "];";
}
