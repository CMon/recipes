#include "database_test.h"

#include <recipes/common/user.h>
#include <recipes/database/database.h>
#include <recipes/database/dbpermission.h>
#include <recipes/database/dbrecipe.h>
#include <recipes/database/dbuser.h>
#include <recipes/database/migrator.h>

#include <shared/util.h>

#include <QSqlDatabase>

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

	Database::setCredentials(databaseTestName, "root", "sql");
	Migrator mig(databaseTestName, "root", "sql");

	QVERIFY(mig.update(":/database/db_scheme.sql"));
}

void DatabaseTest::cleanupTestCase()
{
	Database::closeThreadStoredDatabase();
}

void DatabaseTest::user_updateUser_getAllUsers()
{
	User userToTest("user_updateUser_getAllUsers", Permissions(Permissions::Administrator), "first user_updateUser_getAllUsers", "last user_updateUser_getAllUsers", false);

	QVERIFY(DB::addOrUpdateUser(userToTest, "password"));

	const QList<User> users = DB::getAllUsers();
	QVERIFY(users.count() > 0);
	QCOMPARE(users.first().getLogin(),     userToTest.getLogin());
	QVERIFY(users.first().hasPermission(Permissions::Administrator));
	QCOMPARE(users.first().getFirstName(), userToTest.getFirstName());
	QCOMPARE(users.first().getLastName(),  userToTest.getLastName());
	QCOMPARE(users.first().getIsDeleted(), userToTest.getIsDeleted());
}

void DatabaseTest::user_updateUser_getUser()
{
	User userToTest("user_updateUser_getUser", Permissions(Permissions::Administrator), "first user_updateUser_getUser", "last user_updateUser_getUser", false);

	QVERIFY(DB::addOrUpdateUser(userToTest, "password"));

	const User user = DB::getUser("user_updateUser_getUser");
	QCOMPARE(user.getLogin(),     userToTest.getLogin());
	QVERIFY(user.hasPermission(Permissions::Administrator));
	QCOMPARE(user.getFirstName(), userToTest.getFirstName());
	QCOMPARE(user.getLastName(),  userToTest.getLastName());
	QCOMPARE(user.getIsDeleted(), userToTest.getIsDeleted());
}

void DatabaseTest::user_checkPassword()
{
	User userToTest("user_checkPassword", Permissions(Permissions::Administrator), "first user_checkPassword", "last user_checkPassword", false);
	QVERIFY(DB::addOrUpdateUser(userToTest, "password"));

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
		QCOMPARE(units.first(), updatedUnit);
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
		QCOMPARE(categories.first(), updatedCategory);
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
	DB::addOrUpdateCategory(category);

	// initial Ingredient
	Ingredient ingredient(false, false, false);
	{
		ingredient.setFoodCategory(category);
		ingredient.updateName(QLocale("de_DE"), "Zucker");

		DB::addOrUpdateIngredient(ingredient);

		const QList<Ingredient> ingredients = DB::getIngredients();
		QCOMPARE(ingredients.size(), 1);
		QCOMPARE(ingredients.first(), ingredient);
	}

	// adding another translation to initial Ingredient
	Ingredient updatedIngredient = ingredient;
	{
		updatedIngredient.updateName(QLocale("en_US"), "Desert");
		QVERIFY(ingredient != updatedIngredient);

		DB::addOrUpdateIngredient(updatedIngredient);

		const QList<Ingredient> ingredients = DB::getIngredients();
		QCOMPARE(ingredients.size(), 1);
		QCOMPARE(ingredients.first(), updatedIngredient);
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
		QCOMPARE(portions.first(), portion);
	}

	// adding another translation to initial Portion
	Portion updatedPortion = portion;
	{
		updatedPortion.updateDescriptions(QLocale("en_US"), "Plate");
		QVERIFY(portion != updatedPortion);

		DB::addOrUpdatePortion(updatedPortion);

		const QList<Portion> portions = DB::getPortions();
		QCOMPARE(portions.size(), 1);
		QCOMPARE(portions.first(), updatedPortion);
	}

	// adding another Portion
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

void DatabaseTest::addOrUpdateRecipe_getRecipes()
{
	// adding stuff that is needed for a recipe
	Portion portion;
	portion.updateDescriptions(QLocale("de_DE"), "Schüssel");
	DB::addOrUpdatePortion(portion);
	portion.setCount(2);
	User dummy = User("addOrUpdateRecipe_getRecipes", Permissions(Permissions::Administrator), "first", "last", false);
	QVERIFY(DB::addOrUpdateUser(dummy, "password"));
	const User user = DB::getUser("addOrUpdateRecipe_getRecipes");
	Unit gram(1, 1);
	gram.updateAbbreviation(QLocale("de_DE"), "gr");
	gram.updateCompleteName(QLocale("de_DE"), "gram");
	DB::addOrUpdateUnit(gram);
	Category category(false);
	category.updateName(QLocale("de_DE"), "salzig");
	DB::addOrUpdateCategory(category);
	Ingredient ingredient(false, false, false);
	ingredient.updateName(QLocale("de_DE"), "Salz");
	ingredient.setFoodCategory(category);
	DB::addOrUpdateIngredient(ingredient);

	// initial Recipe
	Recipe recipe;
	recipe.setCreatedByUser(user);
	recipe.setPortion(portion);
	recipe.addIngredient(5, gram, ingredient);
	{
		recipe.updateTitle(QLocale("de_DE"), "Schokokekse");

		recipe.addInstructionStep(InstructionStep(0xFF, Locale2String(QLocale("de_DE"), "Kekse + Schokolade mischen und schon sinds Schokokekse")));
		QVERIFY2(recipe.isValid(), qPrintable(QString("Recipe invalid: %1").arg(recipe.toString())));

		const QString externId = DB::addOrUpdateRecipe(recipe);
		QVERIFY(!externId.isEmpty());
		recipe.setExternId(externId);

		const QList<Recipe> recipes = DB::getRecipes();
		QCOMPARE(recipes.size(), 1);
		QCOMPARE(recipes.first(), recipe);
	}

	// adding another translation to initial Recipe
	Recipe updatedRecipe = recipe;
	{
		updatedRecipe.updateTitle(QLocale("en_US"), "Chocolatecookies");
		recipe.addInstructionStep(InstructionStep(0, Locale2String(QLocale("en_US"), "Cookies + Choclate mix them and you get Chocolatecookies")));
		QVERIFY(recipe != updatedRecipe);

		QVERIFY(!DB::addOrUpdateRecipe(updatedRecipe).isEmpty());

		const QList<Recipe> recipes = DB::getRecipes();
		QCOMPARE(recipes.size(), 1);
		QCOMPARE(recipes.first(), updatedRecipe);
	}

	// adding another Recipe
	Recipe anotherRecipe;
	{
		anotherRecipe.updateTitle(QLocale("de_DE"), "Mett");
		recipe.addInstructionStep(InstructionStep(0, Locale2String(QLocale("de_DE"), "Fleisch in den Fleischwolf => Mett")));
		anotherRecipe.setCreatedByUser(user);
		anotherRecipe.setPortion(portion);
		anotherRecipe.addIngredient(12, gram, ingredient);
		QVERIFY2(anotherRecipe.isValid(), qPrintable(QString("Recipe invalid: %1").arg(anotherRecipe.toString())));

		const QString externId = DB::addOrUpdateRecipe(anotherRecipe);
		QVERIFY(!externId.isEmpty());
		anotherRecipe.setExternId(externId);

		const QList<Recipe> recipes = DB::getRecipes();
		QCOMPARE(recipes.size(), 2);
		QVERIFY(recipes.contains(updatedRecipe));
		QVERIFY(recipes.contains(anotherRecipe));
	}
}

void DatabaseTest::permission_addPermission_data()
{
	QTest::addColumn<QString>("techName");
	QTest::addColumn<bool>("valid");

	QTest::addRow("empty1") << QString()  << false;
	QTest::addRow("empty2") << ""         << false;
	QTest::addRow("name")   << "testName" <<  true;
}

void DatabaseTest::permission_addPermission()
{
	QFETCH(QString, techName);
	QFETCH(bool, valid);

	const int expectedPerms = DB::getPermissions().size() + (valid ? 1 : 0);

	const int permissionId = DB::addPermission(techName);
	const bool createdPermissionValid = permissionId > 0;
	QCOMPARE(createdPermissionValid, valid);

	const QList<PermissionData> perms = DB::getPermissions();
	QCOMPARE(perms.size(), expectedPerms);

	bool contained = false;
	for(const PermissionData & pData: perms) {
		if (pData.techName() == techName) {
			contained = true;
		}
	}
	QCOMPARE(contained, valid);
}

void DatabaseTest::permission_setDescription_data()
{
	QTest::addColumn<Locale2String>("description");
	QTest::addColumn<bool>("valid");

	Locale2String multi(QLocale("de_DE"), "Beschreibung auf Deutsch");
	multi.add(QLocale("en_US"), "Description in simplified English");

	QTest::addRow("empty")   << Locale2String()                                             << false;
	QTest::addRow("filled1") << Locale2String(QLocale("de_DE"), "Beschreibung auf Deutsch") <<  true;
	QTest::addRow("filled2") << multi                                                       <<  true;
}

void DatabaseTest::permission_setDescription()
{
	QFETCH(Locale2String, description);
	QFETCH(bool, valid);

	const int expectedPerms = DB::getPermissions().size() + 1;
	const QString techName = QString("techName_%1__%2").arg(__FUNCTION__).arg(QTest::currentDataTag());

	const int permissionId = DB::addPermission(techName);
	QVERIFY(permissionId > 0);

	QCOMPARE(DB::setPermissionDescription(permissionId, description), valid);

	QList<PermissionData> perms = DB::getPermissions();
	QCOMPARE(perms.size(), expectedPerms);

	if (valid) {
		bool contained = false;
		for(const PermissionData & pData: perms) {
			if (pData.techName() == techName) {
				for (const QLocale & addedLoc: description.keys()) {
					const QString actualDesc = pData.description().value(addedLoc);
					const QString expectedDesc = description.value(addedLoc);
					QCOMPARE(actualDesc, expectedDesc);
					contained = true;
				}
			}
		}
		QCOMPARE(contained, valid);
	}
}

void DatabaseTest::permission_setTitle_data()
{
	QTest::addColumn<Locale2String>("title");
	QTest::addColumn<bool>("valid");

	Locale2String multi(QLocale("de_DE"), "Administrator");
	multi.add(QLocale("en_US"), "Admin");

	QTest::addRow("empty")   << Locale2String() << false;
	QTest::addRow("filled1") << Locale2String(QLocale("de_DE"), "Administrator") <<  true;
	QTest::addRow("filled2") << multi <<  true;
}

void DatabaseTest::permission_setTitle()
{
	QFETCH(Locale2String, title);
	QFETCH(bool, valid);

	const int expectedPerms = DB::getPermissions().size() + 1;
	const QString techName = QString("techName_%1__%2").arg(__FUNCTION__).arg(QTest::currentDataTag());

	const int permissionId = DB::addPermission(techName);
	QVERIFY(permissionId > 0);

	QCOMPARE(DB::setPermissionTitle(permissionId, title), valid);

	QList<PermissionData> perms = DB::getPermissions();
	QCOMPARE(perms.size(), expectedPerms);

	if (valid) {
		bool contained = false;
		for(const PermissionData & pData: perms) {
			if (pData.techName() == techName) {
				for (const QLocale & addedLoc: title.keys()) {
					const QString actualTitle = pData.title().value(addedLoc);
					const QString expectedTitle = title.value(addedLoc);
					QCOMPARE(actualTitle, expectedTitle);
					contained = true;
				}
			}
		}
		QCOMPARE(contained, valid);
	}
}

void DatabaseTest::permission_setTitleAndDescription_data()
{
	QTest::addColumn<Locale2String>("title");
	QTest::addColumn<Locale2String>("description");

	Locale2String multiTitle(QLocale("de_DE"), "Administrator");
	multiTitle.add(QLocale("en_US"), "Admin");
	Locale2String multiDescription(QLocale("de_DE"), "Beschreibung auf Deutsch");
	multiDescription.add(QLocale("en_US"), "Description in simplified English");

	QTest::newRow("empty")             << Locale2String()                                  << Locale2String();
	QTest::newRow("titleAndDesc")      << Locale2String(QLocale("de_DE"), "Administrator") << Locale2String(QLocale("de_DE"), "Hat die erlaubnis administrativ zu arbeiten");
	QTest::newRow("multiTitleAndDesc") << multiTitle                                       << multiDescription;
}

void DatabaseTest::permission_setTitleAndDescription()
{
	QFETCH(Locale2String, title);
	QFETCH(Locale2String, description);

	const int expectedPerms = DB::getPermissions().size() + 1;
	const QString techName = QString("techName_%1__%2").arg(__FUNCTION__).arg(QTest::currentDataTag());

	const int permissionId = DB::addPermission(techName);
	QVERIFY(permissionId > 0);

	DB::setPermissionTitle(permissionId, title);
	DB::setPermissionDescription(permissionId, description);

	QList<PermissionData> perms = DB::getPermissions();
	QCOMPARE(perms.size(), expectedPerms);

	int contained = 0;
	for(const PermissionData & pData: perms) {
		if (pData.techName() == techName) {
			for (const QLocale & addedLoc: title.keys()) {
				const QString actualTitle = pData.title().value(addedLoc);
				const QString expectedTitle = title.value(addedLoc);
				QCOMPARE(actualTitle, expectedTitle);
				contained++;
			}
			for (const QLocale & addedLoc: description.keys()) {
				const QString actualDesc = pData.description().value(addedLoc);
				const QString expectedDesc = description.value(addedLoc);
				QCOMPARE(actualDesc, expectedDesc);
				contained++;
			}
		}
	}
	QCOMPARE(contained, title.size() + description.size());
}

QTEST_MAIN(DatabaseTest)
#include "moc_database_test.cpp"
