#include "recipeservice.h"

#include <recipes/database/dbrecipe.h>
#include <recipes/servercommon/clientinfocache.h>

#include <cereal/archives/json.hpp>
#include <rpclib/common/serializehelper.h>
#include <rpclib/common/types/qlistcereal.h>
#include <rpclib/server/rpcserver.h>

#include <QJsonObject>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(RECIPE_SERVICES)
Q_LOGGING_CATEGORY(RECIPE_SERVICES, "recipe.services.recipe")

RecipeService::RecipeService(QObject * parent)
    : QObject(parent)
{
}

void addPortion(const Portion & portion);
void addRecipe(const Recipe & recipe);


void RecipeService::registerMethods(RPCServer * server)
{
	// connect methods
	server->addCallback("void RecipeService::addUnit(Unit)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		Q_UNUSED(sendingSocket) // could be used for filtering in sendMessage
		cereal::JSONInputArchive archive(stream);
		Unit unitArg;
		archive(unitArg);
		addUnit(unitArg, sendingSocket);
		return QJsonObject();
	}, "Add a unit to the system" );

	server->addCallback("void RecipeService::addPortion(Portion)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		Q_UNUSED(sendingSocket) // could be used for filtering in sendMessage
		cereal::JSONInputArchive archive(stream);
		Portion portionArg;
		archive(portionArg);
		addPortion(portionArg, sendingSocket);
		return QJsonObject();
	}, "Add a potion to the system" );

	server->addCallback("void RecipeService::addRecipe(Recipe)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		Q_UNUSED(sendingSocket) // could be used for filtering in sendMessage
		cereal::JSONInputArchive archive(stream);
		Recipe recipeArg;
		archive(recipeArg);
		addRecipe(recipeArg, sendingSocket);
		return QJsonObject();
	}, "add a recipe to the system" );

	server->addCallback("QList<Unit> RecipeService::getUnits()", [=] (const QJsonValue & request, QWebSocket * sendingSocket) {
		Q_UNUSED(request)
		Q_UNUSED(sendingSocket)

		QJsonValue retval;
		CEREAL_2_DEST(retval, "units", getUnits(sendingSocket));

		return retval;
	}, "get a list of all available units from the server" );

	server->addCallback("QList<Recipe> RecipeService::searchRecipes(QString)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		cereal::JSONInputArchive archive(stream);

		QString searchTerm;
		archive(searchTerm);

		QJsonValue retval;
		CEREAL_2_DEST(retval, "recipes", searchRecipes(sendingSocket, searchTerm));

		return retval;
	}, "get a list of all available units from the server" );
}

QList<Recipe> RecipeService::searchRecipes(QWebSocket * sendingSocket, const QString & searchTerm)
{
	User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	if (!currentUser.isValidForServer()) {
		qWarning() << "rpc rejected because no valid user is logged in";
		return QList<Recipe>();
	}

	return DB::getRecipes(searchTerm);
}

void RecipeService::addUnit(const Unit & unit, QWebSocket * sendingSocket)
{
	User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	DB::addOrUpdateUnit(unit);
}

QList<Unit> RecipeService::getUnits(QWebSocket * sendingSocket)
{
	User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	return DB::getUnits();
}

void RecipeService::addPortion(const Portion & portion, QWebSocket * sendingSocket)
{
	User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	DB::addOrUpdatePortion(portion);
}

void RecipeService::addRecipe(const Recipe & recipe, QWebSocket * sendingSocket)
{
	User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	DB::addOrUpdateRecipe(recipe);
}
