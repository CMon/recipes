#include "recipe.h"

Recipe::Recipe()
{
}

bool Recipe::isValid()
{
	return
	        creatingUser.isValid()    &&
	        !portion.isNull()         &&
	        (title.size() != 0)       &&
	        (description.size() != 0) &&
	        !ingredients.isEmpty();
}

void Recipe::updateTitle(const QLocale & locale, const QString & title)
{
	this->title[locale] = title;
}

void Recipe::updateDescription(const QLocale & locale, const QString & description)
{
	this->description[locale] = description;
}

void Recipe::setPortion(const Portion & portion)
{
	this->portion = portion;
}

void Recipe::setCreatedByUser(const User & user)
{
	creatingUser = user;
}

bool Recipe::operator ==(const Recipe & rhs) const
{
	return
	        externId     == rhs.externId     &&
	        creatingUser == rhs.creatingUser &&
	        portion      == rhs.portion      &&
	        categories   == rhs.categories   &&
	        title        == rhs.title        &&
	        description  == rhs.description  &&
	        imagePaths   == rhs.imagePaths   &&
	        ingredients  == rhs.ingredients
	        ;
}

bool Recipe::operator !=(const Recipe & rhs) const
{
	return !operator ==(rhs);
}

QString Recipe::toString() const
{
	QString retval =
	        "externId: "     + externId                + " "
	        "creatingUser: " + creatingUser.toString() + " "
	        "portion: "      + portion.toString()      + " "
	        "categories: [";
	foreach(const Category & cat, categories) {
		retval += cat.toString() + " ";
	}
	retval +=
	        "] title: [" + title.toString() + "] "
	        "description: [" + description.toString() + "] "
	        "imagePaths: [" + imagePaths.join(";") + "] "
	        "ingredients: [";
	foreach(const IngredientPOD & pod, ingredients) {
		retval += pod.toString() + " ";
	}
	retval += "]";

	return retval;

}
