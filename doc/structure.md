Thoughts about the database structure(tables)
=============================================

Receipts
--------

* units
** id
** inGram
** inMl
* units_i18n 
** PK(FK(unitId), language(zB: en_GB))
** abbreviation
** completeName

* ingridients
** id
** isLiquid    //used for translation of units into gram or ml
** FK(foodCategoryId) // vegan, vegetarian, ...
** containsGluten
** containsLactose
* ingridients_i18n
** PK(FK(ingridientId), language(zB: en_GB))
** name

* quantities
** id
* portions_i18n
** PK(FK(quantityId), language(zB: en_GB))
** description  //Plate, Part, Human

* category
** id
** isFoodCategory
* category_i18n
** PK(FK(categoryId), language(zB: en_GB))
** category  //Desert, Mainmenu, starter, summer, winter,..., vegetarian, vegan, 

* recipes
** id
** FK(quantityId)
** quantityCount
** FK(createdByUserId)

* recipeTitle_i18n
** PK(FK(recipeId), language(de_DE))
** KEY(description:varchar255)

* recipeDescription_i18n
** PK(FK(recipeId), language(de_DE))
** description:Text

* recipePictures
** PK(FK(recipeId), id)
** pathToImg

* ReceiptToIngredients
** PK(FK(recipeId), id)
** FK(ingridientId)
** isOptional
** unitCount
** FK(unitId)

* ReceiptToCategory
** PK(FK(recipeId), FK(categoryId))

Users
-----

* users
** id
** login
** passwordHash
** permissions
** preName
** lastName
** isDeleted

* recipeBooks  // only used for storing foreign recipes
** PK(FK(userId), FK(recipeId))

* ratings
** PK(FK(recipeId), FK(userId))
** rating       // -1 - 5 (-1 unrated, 0 stars to 5 stars)
** notes:text(1024)






