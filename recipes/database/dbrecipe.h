#pragma once

#include <recipes/common/category.h>
#include <recipes/common/ingredient.h>
#include <recipes/common/portion.h>
#include <recipes/common/recipe.h>
#include <recipes/common/unit.h>

namespace DB {

void addOrUpdateUnit(const Unit & unit);
QList<Unit> getUnits(const int & id = -1);

void addOrUpdateCategory(const Category & category);
QList<Category> getCategories(const int & id = -1);

void addOrUpdateIngredient(const Ingredient & ingredient);
QList<Ingredient> getIngredients(const int & id = -1);

void addOrUpdatePortion(const Portion & portion);
QList<Portion> getPortions(const int & id = -1);

QString addOrUpdateRecipe(const Recipe & recipe);
QList<Recipe> getRecipes(const QString & searchTerm);
QList<Recipe> getRecipes(const QVariantList & recipeIdList = QVariantList());

}
