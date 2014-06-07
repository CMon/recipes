#include "ingredient.h"

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
	bool different =
	        foodCategory    == rhs.foodCategory &&
	        isLiquid        == rhs.isLiquid &&
	        containsGluten  == rhs.containsGluten &&
	        containsLactose == rhs.containsLactose &&
	        name.size()     == rhs.name.size();

	foreach (const QLocale & locale, name) {
		different = different && (rhs.name[locale] == name[locale]);
	}

	return different;
}

bool Ingredient::operator !=(const Ingredient & rhs) const
{
	return !operator ==(rhs);
}

QString Ingredient::toString() const
{
	QString retval =
	        "foodCategory: "    + foodCategory.toString()             + "; "
	        "isLiquid: "        + Util::boolToString(isLiquid)        + "; "
	        "containsGluten: "  + Util::boolToString(containsGluten)  + "; "
	        "containsLactose: " + Util::boolToString(containsLactose) + "; "
	        "names: [";
	foreach(const QLocale & locale, name.keys()) {
		retval += locale.name() + "=" + name.value(locale) + " ";
	}
	retval += "]";

	return retval;
}
