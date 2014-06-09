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

	void updateTitle(const QLocale & locale, const QString & title);
	Locale2String getTitles() const { return title; }
	void updateDescription(const QLocale & locale, const QString & description);
	Locale2String getDescriptions() const { return description; }

	QString toString() const;

private:
	User creatingUser;
	Portion portion;
	Categories categories;
	Locale2String title;
	Locale2String description;
	QStringList imagePaths;
	IngredientPODs ingredients;
};
