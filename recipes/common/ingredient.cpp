#include "ingredient.h"

#include <common/util.h>

Ingredient::Ingredient()
    : isLiquid(false),
      containsGluten(false),
      containsLactose(false)
{
}

Ingredient::Ingredient(const bool isLiquid, const bool withGluten, const bool withLactose)
    :
      isLiquid(isLiquid),
      containsGluten(withGluten),
      containsLactose(withLactose)
{
}

void Ingredient::updateName(const QLocale & locale, const QString & name)
{
	this->name[locale] = name;
}

bool Ingredient::operator ==(const Ingredient & rhs) const
{
	return
	        foodCategory    == rhs.foodCategory &&
	        isLiquid        == rhs.isLiquid &&
	        containsGluten  == rhs.containsGluten &&
	        containsLactose == rhs.containsLactose &&
	        name            == rhs.name;
}

bool Ingredient::operator !=(const Ingredient & rhs) const
{
	return !operator ==(rhs);
}

QString Ingredient::toString() const
{
	return
	        "foodCategory: "    + foodCategory.toString()             + "; "
	        "isLiquid: "        + Util::boolToString(isLiquid)        + "; "
	        "containsGluten: "  + Util::boolToString(containsGluten)  + "; "
	        "containsLactose: " + Util::boolToString(containsLactose) + "; "
	        "names: ["          + name.toString()                     + "];";
}
