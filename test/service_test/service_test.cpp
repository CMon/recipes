#include "service_test.h"

#include <recipes/clientcommon/adminserviceinterface.h>
#include <recipes/clientcommon/userserviceinterface.h>
#include <shared/util.h>

#include <QJsonValue>
#include <QSignalSpy>
#include <QTest>

void ServicesTest::initTestCase()
{
	QVERIFY(testServer_.startServer(true));
	QVERIFY(testServer_.insertTestData());

	rpcClient_.setUrl(testServer_.getUrl());
	rpcClient_.connectToServer();
	QVERIFY(rpcClient_.waitForConnected());

	usi_ = new UserServiceInterface(&rpcClient_);
}

void ServicesTest::cleanupTestCase()
{
	testServer_.stopServer();
	rpcClient_.disconnectFromServer();
}

void ServicesTest::cleanup()
{
	cleanupConnection();
}

void ServicesTest::init()
{
	cleanupConnection();
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

void ServicesTest::adminService_permission_Test_data()
{
	QTest::addColumn<User>("userToUse");
	QTest::addColumn<QString>("password");
	QTest::addColumn<bool>("validLogin");
	QTest::addColumn<bool>("callsAllowed");

	QTest::newRow("none") << User() << QString() << false << false;

	QSignalSpy couldAddUserSpy(usi_, &UserServiceInterface::couldAddUser);

	usi_->login("test.user", "4321");
	QTRY_COMPARE(usi_->loggedIn(), true);

	for (int permId = Permissions::FIRST; permId <= Permissions::LAST; ++permId) {
		const User user(QString("test%1").arg(permId), Permissions(Permissions::Permission(permId)), "first", "last");
		const QString password("4321");

		usi_->addUser(user, password);
		QTRY_COMPARE(couldAddUserSpy.count(), 1);
		QCOMPARE(couldAddUserSpy.takeFirst().at(0).toBool(), true);
		couldAddUserSpy.clear();

		QTest::addRow("perm%d", permId) << user << password << true << (permId == Permissions::Administrator);
	}
}

void ServicesTest::adminService_permission_Test()
{
	QFETCH(User, userToUse);
	QFETCH(QString, password);
	QFETCH(bool, validLogin);
	QFETCH(bool, callsAllowed);

	AdminServiceInterface asi(&rpcClient_);

	QSignalSpy permissionsUpdatedSpy(&asi, &AdminServiceInterface::permissionsUpdated);
	QSignalSpy addPermissionSuccesfulSpy(&asi, &AdminServiceInterface::addPermissionSuccesful);
	QSignalSpy addPermissionTitleSuccesfulSpy(&asi, &AdminServiceInterface::addPermissionTitleSuccesful);
	QSignalSpy addPermissionDescriptionSuccesfulSpy(&asi, &AdminServiceInterface::addPermissionDescriptionSuccesful);

	QCOMPARE(usi_->loggedIn(), false);
	usi_->login(userToUse.getLogin(), password);
	QTRY_COMPARE(usi_->loggedIn(), validLogin);

	// get all permissions
	int permissionCount = 0;
	{
		QCOMPARE(permissionsUpdatedSpy.count(), 0);

		asi.getPermissions();
		if (callsAllowed) {
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 1);
			permissionCount = asi.getCachedPermissions().size();
			COMPARE_OP(permissionCount, >, 1);
		} else {
			QTest::qWait(2000);
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 0);
			permissionCount = asi.getCachedPermissions().size();
			QCOMPARE(permissionCount, 0);
		}

		permissionsUpdatedSpy.clear();
	}

	// add permission
	{
		QCOMPARE(addPermissionSuccesfulSpy.count(), 0);

		const QString newTechname = QString("tech_%1").arg(QTest::currentDataTag());
		const Locale2String expectedDesc("en_US", "Description");
		const Locale2String expectedTitle("en_US", "Title");
		asi.addPermission(newTechname, expectedTitle, expectedDesc);

		QTRY_COMPARE(addPermissionSuccesfulSpy.count(), 1);
		QCOMPARE(addPermissionSuccesfulSpy.takeFirst().at(0).toBool(), callsAllowed);

		if (callsAllowed) {
			asi.getPermissions();
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 1);
			QCOMPARE(asi.getCachedPermissions().size(), permissionCount + 1);
		}

		permissionsUpdatedSpy.clear();
		addPermissionSuccesfulSpy.clear();
		addPermissionTitleSuccesfulSpy.clear();
		addPermissionDescriptionSuccesfulSpy.clear();
	}

	const QLocale french("fr_FR");
	const int permIdToUseForChanges = 3;
	// add permission title
	{
		QCOMPARE(addPermissionTitleSuccesfulSpy.count(), 0);

		const QString frTitle = "administrateur";
		asi.setPermissionTitle(permIdToUseForChanges, Locale2String(french, frTitle));

		QTRY_COMPARE(addPermissionTitleSuccesfulSpy.count(), validLogin ? 1 : 0);
		if (validLogin) {
			QCOMPARE(addPermissionTitleSuccesfulSpy.takeFirst().at(0).toBool(), callsAllowed);
		}

		if (callsAllowed) {
			asi.getPermissions();
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 1);
			bool contained = false;
			for (const PermissionData & pd: asi.getCachedPermissions()) {
				if (pd.permissionId() == permIdToUseForChanges) {
					if (pd.title().value(french) ==  frTitle) {
						contained = true;
						break;
					}
				}
			}
			QVERIFY(contained);
		}

		permissionsUpdatedSpy.clear();
		addPermissionTitleSuccesfulSpy.clear();
	}

	// update permission title
	{
		QCOMPARE(addPermissionTitleSuccesfulSpy.count(), 0);

		const QString updatedFrTitle = "Administrateur";
		asi.setPermissionTitle(permIdToUseForChanges, Locale2String(french, updatedFrTitle));

		QTRY_COMPARE(addPermissionTitleSuccesfulSpy.count(), validLogin ? 1 : 0);
		if (validLogin) {
			QCOMPARE(addPermissionTitleSuccesfulSpy.takeFirst().at(0).toBool(), callsAllowed);
		}

		if (callsAllowed) {
			asi.getPermissions();
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 1);
			bool contained = false;
			for (const PermissionData & pd: asi.getCachedPermissions()) {
				if (pd.permissionId() == permIdToUseForChanges) {
					if (pd.title().value(french) ==  updatedFrTitle) {
						contained = true;
						break;
					}
				}
			}
			QVERIFY(contained);
		}

		permissionsUpdatedSpy.clear();
		addPermissionTitleSuccesfulSpy.clear();
	}

	// add description
	{
		QCOMPARE(addPermissionDescriptionSuccesfulSpy.count(), 0);

		const QString frDesc = "un droit d'ajouter des utilisateurs";
		asi.setPermissionDescription(permIdToUseForChanges, Locale2String(french, frDesc));

		QTRY_COMPARE(addPermissionDescriptionSuccesfulSpy.count(), validLogin ? 1 : 0);
		if (validLogin) {
			QCOMPARE(addPermissionDescriptionSuccesfulSpy.takeFirst().at(0).toBool(), callsAllowed);
		}

		if (callsAllowed) {
			asi.getPermissions();
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 1);
			bool contained = false;
			for (const PermissionData & pd: asi.getCachedPermissions()) {
				if (pd.permissionId() == permIdToUseForChanges) {
					if (pd.description().value(french) ==  frDesc) {
						contained = true;
						break;
					}
				}
			}
			QVERIFY(contained);
		}

		permissionsUpdatedSpy.clear();
		addPermissionDescriptionSuccesfulSpy.clear();
	}

	// update description
	{
		QCOMPARE(addPermissionDescriptionSuccesfulSpy.count(), 0);

		const QString updatedFrDesc = "Un droit d'ajouter des utilisateurs";
		asi.setPermissionDescription(permIdToUseForChanges, Locale2String(french, updatedFrDesc));

		QTRY_COMPARE(addPermissionDescriptionSuccesfulSpy.count(), validLogin ? 1 : 0);
		if (validLogin) {
			QCOMPARE(addPermissionDescriptionSuccesfulSpy.takeFirst().at(0).toBool(), callsAllowed);
		}

		if (callsAllowed) {
			asi.getPermissions();
			QTRY_COMPARE(permissionsUpdatedSpy.count(), 1);
			bool contained = false;
			for (const PermissionData & pd: asi.getCachedPermissions()) {
				if (pd.permissionId() == permIdToUseForChanges) {
					if (pd.description().value(french) ==  updatedFrDesc) {
						contained = true;
						break;
					}
				}
			}
			QVERIFY(contained);
		}

		permissionsUpdatedSpy.clear();
		addPermissionDescriptionSuccesfulSpy.clear();
	}
}

void ServicesTest::cleanupConnection()
{
	if (usi_->loggedIn()) {
		usi_->logout();
		QTRY_COMPARE(usi_->loggedIn(), false);
		QTRY_COMPARE(rpcClient_.isConnected(), false);
	}
	if (!rpcClient_.isConnected()) {
		rpcClient_.connectToServer();
		QVERIFY(rpcClient_.waitForConnected());
	}
}

QTEST_MAIN(ServicesTest)
#include "moc_service_test.cpp"
