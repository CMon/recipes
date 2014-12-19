#include "testserver.h"

#include <recipes/services/userservice.h>
#include <recipes/database/dbuser.h>

#include <QHostAddress>
#include <QTest>

#include <cflib/db/db.h>
#include <cflib/http/apiserver.h>
#include <cflib/http/server.h>

User fullUser = User(UserId(0), "FullUser", Permission::getAll(), "Full", "User", false);

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

namespace {
	const QString databaseTestName = "recipes_blackbox_test";
}

TestServer::TestServer()
{
}

TestServer::~TestServer()
{
	stopServer();
}

bool TestServer::startServer(bool initDB)
{
	initTestDatabase(initDB);

	UserService userService;

	cflib::http::ApiServer api;
	api.registerService(&userService);

	const int port = 8666;
	const QHostAddress listenOn = QHostAddress::LocalHost;

	if (serv_.isRunning()) serv_.stop();

	serv_.registerHandler(&api);
	if (!serv_.start(port, listenOn.toString().toLatin1())) {
		return false;
	}

	return true;
}

void TestServer::stopServer()
{
	if (serv_.isRunning()) serv_.stop();
}

bool TestServer::isServerRunning()
{
	return serv_.isRunning();
}

bool TestServer::insertTestUsers()
{
	if (!isServerRunning()) return false;

	// directly add users to table, because we do not have any to login and use the services

	DB::updateUser(fullUser, "1234");

	return true;
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

	cflib::util::Log::start(databaseTestName + ".log");
	cflib::db::setParameter(databaseTestName, "root", "sql");

	if (initDB) {
		QFile file(":/database/db_scheme.sql");
		QVERIFY(file.open(QIODevice::ReadOnly));

		Transaction;
		ta.db.exec(file.readAll());
		QVERIFY(ta.commit());
	}
}
