#include "server.h"

#include <recipes/database/database.h>
#include <recipes/database/migrator.h>

#include <QSettings>

#define SETTING_ORGA_STRING "cmon"
#define SETTING_APP_STRING  "recipes.server"

Server::Server(const QHostAddress & listenOn, quint16 port)
    : rpcServer_(listenOn, port)
{
	loadSettings();

	Database::setCredentials(dbName_, dbUser_, dbPass_);
}

void Server::init()
{
	userService_ = new UserService;
	userService_->registerMethods(&rpcServer_);
	recipeService_ = new RecipeService;
	recipeService_->registerMethods(&rpcServer_);
}

QList<QPair<QString, QString>> Server::availableServiceMethods()
{
	return rpcServer_.availableServiceMethods();
}

bool Server::updateDatabase()
{
	Migrator mig(dbName_, dbUser_, dbPass_);

	if (!mig.update(":/database/db_scheme.sql")) {
		qCritical() << "Update of database failed";
		return false;
	}

	return true;
}

bool Server::start()
{
	return rpcServer_.start();
}

void Server::loadSettings()
{
	const QSettings settings(SETTING_ORGA_STRING, SETTING_APP_STRING);

	dbName_ = settings.value("databaseName", "recipes").toString();
	dbUser_ = settings.value("databaseUser", "root").toString();
	dbPass_ = settings.value("databasePassword", "sql").toString();
}
