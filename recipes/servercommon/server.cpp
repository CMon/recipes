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

void Server::setDbCredentials(const QString & databaseName, const QString & user, const QString & password)
{
	dbName_ = databaseName;
	dbUser_ = user;
	dbPass_ = password;
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

bool Server::insertTestData()
{
#ifndef NDEBUG
	Migrator mig(dbName_, dbUser_, dbPass_);

	if (!mig.addTestData(":/database/test_data.sql")) {
		qCritical() << "Update of database failed";
		return false;
	}
	return true;
#else
	return false;
#endif
}

bool Server::start()
{
	return rpcServer_.start();
}

void Server::stop()
{
	rpcServer_.stop();
}

bool Server::isRunning() const
{
	return rpcServer_.isRunning();
}

QUrl Server::getUrl() const
{
	return rpcServer_.getServerUrl();
}

void Server::loadSettings()
{
	const QSettings settings(SETTING_ORGA_STRING, SETTING_APP_STRING);

	dbName_ = settings.value("databaseName", "recipes").toString();
	dbUser_ = settings.value("databaseUser", "root").toString();
	dbPass_ = settings.value("databasePassword", "sql").toString();
}
