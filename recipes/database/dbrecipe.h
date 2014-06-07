#pragma once

#include <common/unit.h>
#include <common/category.h>
#include <common/ingredient.h>

namespace DB {

void addOrUpdateUnit(const Unit & unit);
QList<Unit> getUnits();

void addOrUpdateCategory(const Category & category);
QList<Category> getCategories(const int & id = -1);

void addOrUpdateIngredient(const Ingredient & ingredient);
QList<Ingredient> getIngredients();
}
