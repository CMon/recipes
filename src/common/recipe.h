#pragma once

#include <common/category.h>
#include <common/ingredient.h>
#include <common/portion.h>
#include <common/user.h>
#include <common/util.h>

#include <QStringList>

class Recipe
{
public:
    Recipe();

private:
    User creatingUser;
    Portion portion;
    Categories categories;
    Locale2String title;
    Locale2String description;
    QStringList imagePaths;
    Ingredients ingredients;
};
