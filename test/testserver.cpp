#include "testserver.h"

#include <recipes/services/userservice.h>
#include <recipes/database/dbuser.h>

#include <QHostAddress>

#include <cflib/http/apiserver.h>
#include <cflib/http/server.h>

User fullUser = User(UserId(0), "FullUser", Permission::getAll(), "Full", "User", false);

TestServer::TestServer()
{
}

TestServer::~TestServer()
{
	stopServer();
}

bool TestServer::startServer()
{
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
