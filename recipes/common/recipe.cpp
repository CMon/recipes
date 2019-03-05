#include "recipe.h"

Recipe::Recipe()
{
}

bool Recipe::isValid() const
{
	return
	        creatingUser.isValidForClient() &&
	        !portion.isNull()               &&
	        (title.size() != 0)             &&
	        !ingredients.isEmpty();
}

void Recipe::updateTitle(const QLocale & locale, const QString & title)
{
	this->title[locale] = title;
}

void Recipe::setPortion(const Portion & portion)
{
	this->portion = portion;
}

void Recipe::setCreatedByUser(const User & user)
{
	creatingUser = user;
}

void Recipe::clearIngredients()
{
	ingredients.clear();
}

void Recipe::addIngredient(const int count, const Unit & unit, const Ingredient & ingredient, const bool isOptional)
{
	ingredients << IngredientPOD(count, unit, ingredient, isOptional);
}

bool Recipe::operator ==(const Recipe & rhs) const
{
	return
	        externId     == rhs.externId     &&
	        creatingUser == rhs.creatingUser &&
	        portion      == rhs.portion      &&
	        categories   == rhs.categories   &&
	        title        == rhs.title        &&
	        imagePath    == rhs.imagePath    &&
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
	        "] title: " + title.toString() + " "
	        "imagePaths: " + imagePath + " "
	        "ingredients: [";
	foreach(const IngredientPOD & pod, ingredients) {
		retval += pod.toString() + " ";
	}
	retval += "]";

	return retval;

}

QString Recipe::getImagePath() const
{
	return imagePath;
}

void Recipe::setImagePath(const QString & value)
{
	imagePath = value;
}

void Recipe::addInstructionStep(InstructionStep step)
{
	const quint8 position = step.getPosition();
	if (position >= ingredients.size()) {
		step.setPosition(static_cast<quint8>(ingredients.size()));
		instructionSteps.append(step);
	} else {
		InstructionStep origStep = instructionSteps.at(position);
		origStep.addDescription(step.getDescription());
		if (!step.getPicturePath().isEmpty()) origStep.setPicturePath(step.getPicturePath());

		instructionSteps.insert(position, origStep);
	}
}

QList<InstructionStep> Recipe::getInstructionSteps() const
{
	return instructionSteps;
}

void Recipe::setInstructionSteps(const QList<InstructionStep> & value)
{
	instructionSteps = value;
}
