#pragma once

#include <recipes/common/category.h>
#include <recipes/common/ingredient.h>
#include <recipes/common/portion.h>
#include <recipes/common/user.h>
#include <recipes/common/util.h>

#include <rpclib/common/types/types.h>

#include <cereal/cereal.hpp>

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

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("externId", externId),
		   cereal::make_nvp("creatingUser", creatingUser),
		   cereal::make_nvp("portion", portion),
		   cereal::make_nvp("categories", categories),
		   cereal::make_nvp("title", title),
		   cereal::make_nvp("description", description),
		   cereal::make_nvp("imagePaths", imagePaths),
		   cereal::make_nvp("ingredients", ingredients)
		   );
	}

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
CEREAL_CLASS_VERSION(Recipe, 1);
