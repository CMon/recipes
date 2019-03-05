#pragma once

#include <recipes/common/portion.h>
#include <recipes/common/unit.h>
#include <recipes/common/recipe.h>

class RPCServer;
class QWebSocket;

class RecipeService : public QObject
{
	Q_OBJECT

public:
	RecipeService(QObject *parent = nullptr);

	void registerMethods(RPCServer * server);

public: // rpc methods
	QList<Recipe> searchRecipes(QWebSocket * sendingSocket, const QString & searchTerm);
	void addUnit(const Unit & unit, QWebSocket * sendingSocket);
	QList<Unit> getUnits(QWebSocket * sendingSocket);
	void addPortion(const Portion & portion, QWebSocket * sendingSocket);
	void addRecipe(const Recipe & recipe, QWebSocket * sendingSocket);
};
