#include "ingredient.h"

Ingredient::Ingredient(const Locale2String & name, const Unit & unit, const Category & category)
    : name(name),
      unit(unit),
      foodCategory(category),
      isLiquid(false),
      containsGluten(false),
      containsLactose(false),
      isOptional(false)
{
}
