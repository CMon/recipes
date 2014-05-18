#include "database_test.h"

#include <database/dbuser.h>
#include <common/user.h>

#include <cflib/db/db.h>
#include <cflib/util/test.h>

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

namespace {
	const QString databaseTestName = "recipes_database_test";
}

void DatabaseTest::initTestCase()
{
	cflib::util::Log::start(databaseTestName + ".log");

	cflib::db::setParameter(databaseTestName, "root", "sql");

	QFile file(":/database/db_scheme.sql");
	QVERIFY(file.open(QIODevice::ReadOnly));

	Transaction;
	ta.db.exec(file.readAll());
	ta.commit();
}

void DatabaseTest::cleanupTestCase()
{
	Transaction;
	ta.db.exec("DROP DATABASE " + databaseTestName + "; CREATE DATABASE " + databaseTestName + ";");
	ta.commit();

	cflib::db::closeDBConnection();
}

void DatabaseTest::user_updateUser_getAllUsers()
{
	const User userToTest(-1, "user_updateUser_getAllUsers", Permission(Permission::Admin), "first user_updateUser_getAllUsers", "last user_updateUser_getAllUsers", false);

	QVERIFY(DB::updateUser(userToTest, "password"));

	const QList<User> users = DB::getAllUsers();
	QVERIFY(users.count() > 0);
	QCOMPARE(users.first().getLogin(),     userToTest.getLogin());
	QVERIFY(users.first().hasPermission(Permission::Admin));
	QCOMPARE(users.first().getFirstName(), userToTest.getFirstName());
	QCOMPARE(users.first().getLastName(),  userToTest.getLastName());
	QCOMPARE(users.first().getIsDeleted(), userToTest.getIsDeleted());
}

void DatabaseTest::user_updateUser_getUser()
{
	const User userToTest(-1, "user_updateUser_getUser", Permission(Permission::Admin), "first user_updateUser_getUser", "last user_updateUser_getUser", false);

	QVERIFY(DB::updateUser(userToTest, "password"));

	const User user = DB::getUser("user_updateUser_getUser");
	QCOMPARE(user.getLogin(),     userToTest.getLogin());
	QVERIFY(user.hasPermission(Permission::Admin));
	QCOMPARE(user.getFirstName(), userToTest.getFirstName());
	QCOMPARE(user.getLastName(),  userToTest.getLastName());
	QCOMPARE(user.getIsDeleted(), userToTest.getIsDeleted());
}

void DatabaseTest::user_checkPassword()
{
	const User userToTest(-1, "user_checkPassword", Permission(Permission::Admin), "first user_checkPassword", "last user_checkPassword", false);
	QVERIFY(DB::updateUser(userToTest, "password"));

	QVERIFY(DB::checkPassword("user_checkPassword", "password"));
	QVERIFY(!DB::checkPassword("user_checkPassword", "hallo welt"));
	QVERIFY(!DB::checkPassword("user_checkPassword", "bla"));
	QVERIFY(!DB::checkPassword("user_checkPassword", "passwor"));
}

#include "moc_database_test.cpp"
ADD_TEST(DatabaseTest)
