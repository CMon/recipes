#include "testserver.h"

#include <recipes/database/database.h>
#include <recipes/database/dbuser.h>
#include <recipes/services/userservice.h>

#include <QHostAddress>
#include <QTest>
#include <QSqlDatabase>

namespace {
	const QString databaseTestName = "recipes_blackbox_test";
}

TestServer::TestServer()
    : server_(QHostAddress::LocalHost, 8666)
{
	server_.init();
}

TestServer::~TestServer()
{
	stopServer();
}

bool TestServer::startServer(bool initDB)
{
	stopServer();

	initTestDatabase(initDB);

	if (!server_.start()) {
		return false;
	}

	return true;
}

void TestServer::stopServer()
{
	if (server_.isRunning()) server_.stop();
}

bool TestServer::isServerRunning()
{
	return server_.isRunning();
}

bool TestServer::insertTestData()
{
	if (!isServerRunning()) return false;

	return server_.insertTestData();
}

QUrl TestServer::getUrl() const
{
	return server_.getUrl();
}

static void resetDatabase()
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setUserName("root");
	db.setPassword("sql");
	db.open();
	db.exec("DROP DATABASE " + databaseTestName);
	db.exec("CREATE DATABASE " + databaseTestName);
	db.close();
}

void TestServer::initTestDatabase(bool initDB)
{
	if (initDB) resetDatabase();

	Database::setCredentials(databaseTestName, "root", "sql");
	server_.setDbCredentials(databaseTestName, "root", "sql");
	if (initDB) {
		if (!server_.updateDatabase())
		{
			qFatal("Could not update database to newest version, aborting");
		}
	}
}
