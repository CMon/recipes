#pragma once

#include <test/shared/testserver.h>
#include <rpclib/client/rpcclient.h>

#include <QTest>

class UserServiceInterface;

class ServicesTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();
	void cleanup();
	void init();

	void userServiceTest();
	void adminService_permission_Test_data();
	void adminService_permission_Test();

private:
	void cleanupConnection();

private:
	TestServer testServer_;
	RPCClient rpcClient_;
	UserServiceInterface * usi_;
};
