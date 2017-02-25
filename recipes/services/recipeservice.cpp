#include "recipeservice.h"

#include <recipes/servercommon/clientinfocache.h>

RecipeService::RecipeService()
    : RMIService(serializeTypeInfo().typeName)
{
}

void RecipeService::addUnit(const Unit & unit)
{
#warning fill me with logic
}

QList<Unit> RecipeService::getUnits()
{
#warning fill me with logic
}

void RecipeService::addPortion(const Portion & portion)
{
#warning fill me with logic
}

void RecipeService::addRecipe(const Recipe & recipe)
{
#warning fill me with logic
}

void RecipeService::preCallInit()
{
	currentUser_ = ClientInfoCache::instance().getUser(connId());
}
