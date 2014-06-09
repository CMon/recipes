#include "recipe.h"

Recipe::Recipe()
{
}

void Recipe::updateTitle(const QLocale & locale, const QString & title)
{
	this->title[locale] = title;
}

void Recipe::updateDescription(const QLocale & locale, const QString & description)
{
	this->description[locale] = description;
}

QString Recipe::toString() const
{
	QString retval =
			"creatingUser: " + creatingUser.toString() + " "
			"portion: "      + portion.toString() + " ";

	retval += "categories: [";
	foreach(const Category & cat, categories) {
		retval += cat.toString() + " ";
	}
	retval += "] title: [";
	foreach(const QLocale & locale, title.keys()) {
		retval += locale.name() + "=" + title.value(locale) + " ";
	}
	retval += "] description: [";
	foreach(const QLocale & locale, description.keys()) {
		retval += locale.name() + "=" + description.value(locale) + " ";
	}
	retval += "]";
	retval += "imagePaths: [" + imagePaths.join(";") + "] ";
	retval += "ingredients: [";
	foreach(const IngredientPOD & pod, ingredients) {
		retval += pod.toString() + " ";
	}
	retval += "]";

	return retval;

}
