#pragma once

#include <test/shared/testserver.h>
#include <rpclib/client/rpcclient.h>

#include <QTest>

class ServicesTest : public QObject
{
	Q_OBJECT
private slots:
	void initTestCase();
	void cleanupTestCase();

	void userServiceTest();

private:
	TestServer testServer_;
	RPCClient rpcClient_;
};
