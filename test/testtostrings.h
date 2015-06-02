#pragma once

#include <recipes/common/category.h>
#include <recipes/common/ingredient.h>
#include <recipes/common/portion.h>
#include <recipes/common/recipe.h>
#include <recipes/common/unit.h>

namespace QTest {

template<>
char * toString(const int & number)
{
	return qstrdup(qPrintable(QString::number(number)));
}


template<>
char * toString(const Category & category)
{
	return qstrdup(qPrintable(category.toString()));
}

template<>
char * toString(const Ingredient & ingredient)
{
	return qstrdup(qPrintable(ingredient.toString()));
}

template<>
char * toString(const Portion & portion)
{
	return qstrdup(qPrintable(portion.toString()));
}

template<>
char * toString(const Recipe & recipe)
{
	return qstrdup(qPrintable(recipe.toString()));
}

template<>
char * toString(const Unit & unit)
{
	return qstrdup(qPrintable(unit.toString()));
}

}
