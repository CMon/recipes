#include "service_test.h"

#include <recipes/clientcommon/userserviceinterface.h>

#include <QSignalSpy>
#include <QTest>

void ServicesTest::initTestCase()
{
	QVERIFY(testServer_.startServer(true));
	QVERIFY(testServer_.insertTestData());

	rpcClient_.setUrl(testServer_.getUrl());
	rpcClient_.connectToServer();
	QVERIFY(rpcClient_.waitForConnected());
}

void ServicesTest::cleanupTestCase()
{
	testServer_.stopServer();
	rpcClient_.disconnectFromServer();
}

void ServicesTest::userServiceTest()
{
	UserServiceInterface usi(&rpcClient_);

	const QString newUserName = "new";
	const User userToAdd(newUserName, Permissions(Permissions::Administrator), "New", "User");
	const QString passForUser = "newPass";

	QSignalSpy couldAddUserSpy(&usi, &UserServiceInterface::couldAddUser);
	QSignalSpy listOfUsersReceivedSpy(&usi, &UserServiceInterface::listOfUsersReceived);

	// try to add a user even if we are not logged in
	{
		QCOMPARE(couldAddUserSpy.count(), 0);
		usi.addUser(userToAdd, passForUser);
		QTRY_COMPARE(couldAddUserSpy.count(), 1);
		QCOMPARE(couldAddUserSpy.takeFirst().at(0).toBool(), false);
	}

	QCOMPARE(usi.loggedIn(), false);
	usi.login("test.user", "4321");
	QTRY_COMPARE(usi.loggedIn(), true);

	// try to add the user again
	{
		QCOMPARE(couldAddUserSpy.count(), 0);
		usi.addUser(userToAdd, passForUser);
		QTRY_COMPARE(couldAddUserSpy.count(), 1);
		QCOMPARE(couldAddUserSpy.takeFirst().at(0).toBool(), true);
	}

	// check if the list of users contains our new one
	{
		QCOMPARE(listOfUsersReceivedSpy.count(), 0);
		usi.getAllUsers();
		QTRY_COMPARE(listOfUsersReceivedSpy.count(), 1);
		QList<User> users = qvariant_cast<QList<User>>(listOfUsersReceivedSpy.takeFirst().at(0));
		QCOMPARE(users.size(), 3);
		bool foundUser = false;
		for (const User & user: users) {
			if (user.getLogin() == newUserName) {
				foundUser = true;
				break;
			}
		}
		QVERIFY(foundUser);
	}

	// call logout() => success
	QCOMPARE(usi.loggedIn(), true);
	usi.logout();
	QTRY_COMPARE(usi.loggedIn(), false);
	rpcClient_.connectToServer();
	QVERIFY(rpcClient_.waitForConnected());

	// try to add the user again
	{
		QCOMPARE(couldAddUserSpy.count(), 0);
		usi.addUser(userToAdd, passForUser);
		QTRY_COMPARE(couldAddUserSpy.count(), 1);
		QCOMPARE(couldAddUserSpy.takeFirst().at(0).toBool(), false);
	}

	// login with the new user
	QCOMPARE(usi.loggedIn(), false);
	usi.login(newUserName, passForUser);
	QTRY_COMPARE(usi.loggedIn(), true);

	// get a list of users with the new user
	{
		QCOMPARE(listOfUsersReceivedSpy.count(), 0);
		usi.getAllUsers();
		QTRY_COMPARE(listOfUsersReceivedSpy.count(), 1);
		const QList<User> users = qvariant_cast<QList<User>>(listOfUsersReceivedSpy.takeFirst().at(0));
		QCOMPARE(users.size(), 3);
	}
}

QTEST_MAIN(ServicesTest)
#include "moc_service_test.cpp"
