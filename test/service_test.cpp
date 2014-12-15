#include "service_test.h"

#include <cflib/util/test.h>


void ServicesTest::initTestCase()
{
	testServer_.startServer();
	testServer_.insertTestUsers();
}

void ServicesTest::cleanupTestCase()
{
	testServer_.stopServer();
}

void ServicesTest::userServiceTest()
{
	// try to call userHandler.addUser() => fail

	// call login() => success

	// try to call userHandler.addUser() => success

	// call logout() => success

	// try to call userHandler.addUser() => fail
}

#include "moc_service_test.cpp"
ADD_TEST(ServicesTest)
