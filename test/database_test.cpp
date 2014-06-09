#include "database_test.h"

#include <database/dbuser.h>
#include <database/dbrecipe.h>
#include <common/user.h>

#include <cflib/db/db.h>
#include <cflib/util/test.h>

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

namespace {
	const QString databaseTestName = "recipes_database_test";
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

void DatabaseTest::initTestCase()
{
	resetDatabase();

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

void DatabaseTest::addOrUpdateUnit_getUnits()
{
	// initial unit
	Unit unit(28, -1);
	{
		unit.updateAbbreviation(QLocale("de_DE"), "oz");
		unit.updateCompleteName(QLocale("de_DE"), "Unze");

		DB::addOrUpdateUnit(unit);

		const QList<Unit> units = DB::getUnits();
		QCOMPARE(units.size(), 1);
		QCOMPARE(units.first(), unit);
	}

	// adding another translation to initial unit
	Unit updatedUnit = unit;
	{
		updatedUnit.updateAbbreviation(QLocale("en_US"), "oz");
		updatedUnit.updateCompleteName(QLocale("en_US"), "Ounce");
		QVERIFY(unit != updatedUnit);

		DB::addOrUpdateUnit(updatedUnit);

		const QList<Unit> units = DB::getUnits();
		QCOMPARE(units.size(), 1);
		COMPARE(units.first(), updatedUnit);
	}

	// adding another unit
	Unit anotherUnit(1, 1);
	{
		anotherUnit.updateAbbreviation(QLocale("de_DE"), "ml");
		anotherUnit.updateCompleteName(QLocale("de_DE"), "milliliter");

		DB::addOrUpdateUnit(anotherUnit);

		const QList<Unit> units = DB::getUnits();
		QCOMPARE(units.size(), 2);
		QVERIFY(units.contains(updatedUnit));
		QVERIFY(units.contains(anotherUnit));
	}
}

void DatabaseTest::addOrUpdateCategory_getCategories()
{
	// initial category
	Category category(true);
	{
		category.updateName(QLocale("de_DE"), "Nachtisch");

		DB::addOrUpdateCategory(category);

		const QList<Category> categories = DB::getCategories();
		QCOMPARE(categories.size(), 1);
		QCOMPARE(categories.first(), category);
	}

	// adding another translation to initial category
	Category updatedCategory = category;
	{
		updatedCategory.updateName(QLocale("en_US"), "Desert");
		QVERIFY(category != updatedCategory);

		DB::addOrUpdateCategory(updatedCategory);

		const QList<Category> categories = DB::getCategories();
		QCOMPARE(categories.size(), 1);
		COMPARE(categories.first(), updatedCategory);
	}

	// adding another category
	Category anotherCategory(false);
	{
		anotherCategory.updateName(QLocale("de_DE"), "süss");

		DB::addOrUpdateCategory(anotherCategory);

		const QList<Category> categories = DB::getCategories();
		QCOMPARE(categories.size(), 2);
		QVERIFY(categories.contains(updatedCategory));
		QVERIFY(categories.contains(anotherCategory));
	}
}

void DatabaseTest::addOrUpdateIngredient_getIngredients()
{
	// needed for the test elements
	Category category(false);
	category.updateName(QLocale("de_DE"), "süss");

	// initial Ingredient
	Ingredient ingredient(false, false, false);
	{
		ingredient.setFoodCategory(category);
		ingredient.updateName(QLocale("de_DE"), "Zucker");

		DB::addOrUpdateIngredient(ingredient);

		const QList<Ingredient> ingredients = DB::getIngredients();
		QCOMPARE(ingredients.size(), 1);
		COMPARE(ingredients.first(), ingredient);
	}

	// adding another translation to initial Ingredient
	Ingredient updatedIngredient = ingredient;
	{
		updatedIngredient.updateName(QLocale("en_US"), "Desert");
		QVERIFY(ingredient != updatedIngredient);

		DB::addOrUpdateIngredient(updatedIngredient);

		const QList<Ingredient> ingredients = DB::getIngredients();
		QCOMPARE(ingredients.size(), 1);
		COMPARE(ingredients.first(), updatedIngredient);
	}

	// adding another Ingredient
	Ingredient anotherIngredient(true, false, false);
	{
		anotherIngredient.setFoodCategory(category);
		anotherIngredient.updateName(QLocale("de_DE"), "Milch");

		DB::addOrUpdateIngredient(anotherIngredient);

		const QList<Ingredient> ingredients = DB::getIngredients();
		QCOMPARE(ingredients.size(), 2);
		QVERIFY(ingredients.contains(updatedIngredient));
		QVERIFY(ingredients.contains(anotherIngredient));
	}
}

void DatabaseTest::addOrUpdatePortion_getPortions()
{
	// initial Portion
	Portion portion;
	{
		portion.updateDescriptions(QLocale("de_DE"), "Teller");

		DB::addOrUpdatePortion(portion);

		const QList<Portion> portions = DB::getPortions();
		QCOMPARE(portions.size(), 1);
		COMPARE(portions.first(), portion);
	}

	// adding another translation to initial Portion
	Portion updatedPortion = portion;
	{
		updatedPortion.updateDescriptions(QLocale("en_US"), "Plate");
		QVERIFY(portion != updatedPortion);

		DB::addOrUpdatePortion(updatedPortion);

		const QList<Portion> portions = DB::getPortions();
		QCOMPARE(portions.size(), 1);
		COMPARE(portions.first(), updatedPortion);
	}

	// adding another Ingredient
	Portion anotherPortion;
	{
		anotherPortion.updateDescriptions(QLocale("de_DE"), "Person(en)");

		DB::addOrUpdatePortion(anotherPortion);

		const QList<Portion> portions = DB::getPortions();
		QCOMPARE(portions.size(), 2);
		QVERIFY(portions.contains(updatedPortion));
		QVERIFY(portions.contains(anotherPortion));
	}
}


#include "moc_database_test.cpp"
ADD_TEST(DatabaseTest)
