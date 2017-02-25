#pragma once

#include <recipes/common/portion.h>
#include <recipes/common/unit.h>
#include <recipes/common/recipe.h>

#include <cflib/net/rmiservice.h>

class RecipeService : public QObject, public cflib::net::RMIService<QString>
{
	Q_OBJECT
	SERIALIZE_CLASS

public:
	RecipeService();

rmi:
	void addUnit(const Unit & unit);
	QList<Unit> getUnits();
	void addPortion(const Portion & portion);
	void addRecipe(const Recipe & recipe);

protected:
	virtual void preCallInit();

private:
	User currentUser_;
};
