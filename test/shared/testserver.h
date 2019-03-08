#pragma once

#include <recipes/common/user.h>
#include <recipes/servercommon/server.h>

// Test globals
extern User fullUser;

class TestServer
{
public:
	TestServer();
	~TestServer();

	bool startServer(bool initDB);
	void stopServer();

	bool insertTestData();

private:
	bool isServerRunning();
	void initTestDatabase(bool initDB);

private:
	Server server_;
};
