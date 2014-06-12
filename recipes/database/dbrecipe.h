#pragma once

#include <common/category.h>
#include <common/ingredient.h>
#include <common/portion.h>
#include <common/recipe.h>
#include <common/unit.h>

namespace DB {

void addOrUpdateUnit(const Unit & unit);
QList<Unit> getUnits();

void addOrUpdateCategory(const Category & category);
QList<Category> getCategories(const int & id = -1);

void addOrUpdateIngredient(const Ingredient & ingredient);
QList<Ingredient> getIngredients();

void addOrUpdatePortion(const Portion & portion);
QList<Portion> getPortions(const int & id = -1);

bool addOrUpdateRecipe(const Recipe & recipe);
QList<Recipe> getRecipes();

}
