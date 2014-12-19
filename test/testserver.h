#pragma once

#include <recipes/common/user.h>

#include <cflib/http/server.h>

// Test globals
extern User fullUser;

class TestServer
{
public:
	TestServer();
	~TestServer();

	bool startServer(bool resetDatabase = false);
	void stopServer();

	bool insertTestUsers();

private:
	bool isServerRunning();
	void initTestDatabase(bool initDB);

private:
	cflib::http::Server serv_;
};
