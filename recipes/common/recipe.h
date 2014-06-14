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

	bool isValid() const;

	void setExternId(const QString & eId) { externId = eId; }
	QString getExternId() const { return externId; }

	void updateTitle(const QLocale & locale, const QString & title);
	Locale2String getTitles() const { return title; }
	void updateDescription(const QLocale & locale, const QString & description);
	Locale2String getDescriptions() const { return description; }

	void setPortion(const Portion & portion);
	Portion getPortion() const { return portion; }
	void setCreatedByUser(const User & user);
	User getCreatedByUser() const { return creatingUser; }

	void clearIngredients();
	void addIngredient(const int count, const Unit & unit, const Ingredient & ingredient, const bool isOptional = false);
	IngredientPODs getIngredients() const { return ingredients; }

	bool operator ==(const Recipe & rhs) const;
	bool operator !=(const Recipe & rhs) const;

	QString toString() const;

private:
	QString externId;
	User creatingUser;
	Portion portion;
	Categories categories;
	Locale2String title;
	Locale2String description;
	QStringList imagePaths;
	IngredientPODs ingredients;
};
