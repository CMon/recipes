#include "dbrecipe.h"

#include <cflib/db/db.h>

#include <QLocale>
#include <QHash>

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

// units

static int getUnitId(const Locale2String & abbreviations)
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "unitId "
	            "FROM "
	              "units_i18n "
	            "WHERE "
	             "language = :language AND abbreviation = :abbreviation"
	            );

	int retval = -1;
	foreach (const QLocale & lang, abbreviations.keys()) {
		query.bindValue(":language",     lang.name());
		query.bindValue(":abbreviation", abbreviations.value(lang));

		if (!execQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdateUnitTranslations(const int unitId, const Unit & unit)
{
	Transaction;

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "units_i18n "
	        "("
	          "unitId, language, abbreviation "
	        ") VALUES ("
	          ":unitId, :language, :abbreviation "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "abbreviation=VALUES(abbreviation) "
	            );

	queryAbbrev.bindValue(":unitId", unitId);

	Locale2String abbrevs = unit.getAbbreviations();
	foreach(const QLocale & locale, abbrevs.keys()) {
		queryAbbrev.bindValue(":language",     locale.name());
		queryAbbrev.bindValue(":abbreviation", abbrevs.value(locale));
		execQuery(queryAbbrev);
	}

	QSqlQuery queryName(ta.db);
	queryName.prepare(
	        "INSERT INTO "
	          "units_i18n "
	        "("
	          "unitId, language, completeName "
	        ") VALUES ("
	          ":unitId, :language, :completeName "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "completeName=VALUES(completeName) "
	            );

	queryName.bindValue(":unitId", unitId);

	Locale2String names = unit.getCompleteNames();
	foreach(const QLocale & locale, names.keys()) {
		queryName.bindValue(":language",     locale.name());
		queryName.bindValue(":completeName", names.value(locale));
		execQuery(queryName);
	}

	return ta.commit();
}

void DB::addOrUpdateUnit(const Unit & unit)
{
	Transaction;

	int unitId = getUnitId(unit.getAbbreviations());

	QString queryString;
	if (unitId == -1) {
		queryString =
		        "INSERT INTO "
		          "units "
		        "("
		          "inGram, inMl"
		        ") VALUES ("
		          ":inGram, :inMl"
		        ")";
	} else {
		queryString =
		        "UPDATE "
		          "units "
		        "SET "
		          "inGram=:inGram, "
		          "inMl=:inMl "
		        "WHERE id=:unitId ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryString);

	query.bindValue(":inGram", unit.getGram());
	query.bindValue(":inMl",   unit.getMl());
	if (unitId != -1) {
		query.bindValue(":unitId", unitId);
	}
	if(!execQuery(query)) return;

	if (unitId == -1) {
		unitId = query.lastInsertId().toUInt();
	}

	if (!addOrUpdateUnitTranslations(unitId, unit)) return;

	ta.commit();
}

QList<Unit> DB::getUnits()
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "u.id, u.inGram, u.inMl, i18n.language, i18n.abbreviation, i18n.completeName "
	            "FROM "
	              "units u, units_i18n i18n "
	            "WHERE "
	               "u.id = i18n.unitId"
	            );

	QList<Unit> retval;
	if (!execQuery(query)) return retval;

	QHash<int, Unit> dummy;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(3).toString());

		Unit unit;
		if (dummy.contains(id)) {
			unit = dummy.value(id);
		} else {
			unit = Unit(query.value(1).toInt(), query.value(2).toInt());
		}
		unit.updateAbbreviation(lang, query.value(4).toString());
		unit.updateCompleteName(lang, query.value(5).toString());

		dummy.insert(id, unit);
	}

	ta.commit();

	return dummy.values();
}

// categories

static int getCategoryId(const Locale2String & names)
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "categoryId "
	            "FROM "
	              "categories_i18n "
	            "WHERE "
	              "language = :language AND name = :name"
	            );

	int retval = -1;
	foreach (const QLocale & lang, names.keys()) {
		query.bindValue(":language", lang.name());
		query.bindValue(":name",     names.value(lang));
		if (!execQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdateCategoryTranslation(const int categoryId, const Category & category)
{
	Transaction;

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "categories_i18n "
	        "("
	          "categoryId, language, name "
	        ") VALUES ("
	          ":categoryId, :language, :name "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "name=VALUES(name) "
	            );

	queryAbbrev.bindValue(":categoryId", categoryId);

	Locale2String names = category.getNames();
	foreach(const QLocale & locale, names.keys()) {
		queryAbbrev.bindValue(":language", locale.name());
		queryAbbrev.bindValue(":name",     names.value(locale));
		execQuery(queryAbbrev);
	}

	return ta.commit();
}

void DB::addOrUpdateCategory(const Category & category)
{
	Transaction;

	int categoryId = getCategoryId(category.getNames());

	QString queryString;
	if (categoryId == -1) {
		queryString =
		        "INSERT INTO "
		          "categories "
		        "("
		          "isFoodCategory"
		        ") VALUES ("
		          ":isFoodCategory"
		        ")";
	} else {
		queryString =
		        "UPDATE "
		          "categories "
		        "SET "
		          "isFoodCategory=:isFoodCategory "
		        "WHERE id=:categoryId ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryString);

	query.bindValue(":isFoodCategory", category.isFoodCategory());
	if (categoryId != -1) {
		query.bindValue(":categoryId", categoryId);
	}
	if(!execQuery(query)) return;

	if (categoryId == -1) {
		categoryId = query.lastInsertId().toUInt();
	}

	if (!addOrUpdateCategoryTranslation(categoryId, category)) return;

	ta.commit();
}

QList<Category> DB::getCategories(const int & id)
{
	Transaction;

	QString queryStr =
	        "SELECT "
	          "c.id, c.isFoodCategory, i18n.language, i18n.name "
	        "FROM "
	          "categories c, categories_i18n i18n "
	        "WHERE "
	          "c.id = i18n.categoryId";

	if (id != -1) {
		queryStr += " AND c.id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	QList<Category> retval;
	if (!execQuery(query)) return retval;

	QHash<int, Category> dummy;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(2).toString());

		Category category;
		if (dummy.contains(id)) {
			category = dummy.value(id);
		} else {
			category = Category(query.value(1).toBool());
		}
		category.updateName(lang, query.value(3).toString());

		dummy.insert(id, category);
	}

	ta.commit();

	return dummy.values();
}

// Ingridients

static int getIngredientId(const Locale2String & names)
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "ingredientId "
	            "FROM "
	              "ingredients_i18n "
	            "WHERE "
	              "language = :language AND name = :name"
	            );

	int retval = -1;
	foreach (const QLocale & lang, names.keys()) {
		query.bindValue(":language", lang.name());
		query.bindValue(":name",     names.value(lang));
		if (!execQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdateIngredientTranslation(const int ingredientId, const Ingredient & ingredient)
{
	Transaction;

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "ingredients_i18n "
	        "("
	          "ingredientId, language, name "
	        ") VALUES ("
	          ":ingredientId, :language, :name "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "name=VALUES(name) "
	            );

	queryAbbrev.bindValue(":ingredientId", ingredientId);

	Locale2String names = ingredient.getNames();
	foreach(const QLocale & locale, names.keys()) {
		queryAbbrev.bindValue(":language", locale.name());
		queryAbbrev.bindValue(":name",     names.value(locale));
		execQuery(queryAbbrev);
	}

	return ta.commit();
}

void DB::addOrUpdateIngredient(const Ingredient & ingredient)
{
	Transaction;

	int ingredientId = getIngredientId(ingredient.getNames());

	QString queryString;
	if (ingredientId == -1) {
		queryString =
		        "INSERT INTO "
		          "ingredients "
		        "("
		          "isLiquid, foodCategoryId, containsGluten, containsLactose"
		        ") VALUES ("
		          ":isLiquid, :foodCategoryId, :containsGluten, :containsLactose"
		        ")";
	} else {
		queryString =
		        "UPDATE "
		          "ingredients "
		        "SET "
		          "isLiquid=:isLiquid, "
		          "foodCategoryId=:foodCategoryId, "
		          "containsGluten=:containsGluten, "
		          "containsLactose=:ingredientId "
		        "WHERE id=:categoryId ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryString);

	query.bindValue(":isLiquid",        ingredient.getIsLiquid());
	query.bindValue(":foodCategoryId",  getCategoryId(ingredient.getFoodCategory().getNames()));
	query.bindValue(":containsGluten",  ingredient.getContainsGluten());
	query.bindValue(":containsLactose", ingredient.getContainsLactose());
	if (ingredientId != -1) {
		query.bindValue(":categoryId", ingredientId);
	}
	if(!execQuery(query)) return;

	if (ingredientId == -1) {
		ingredientId = query.lastInsertId().toUInt();
	}

	if (!addOrUpdateIngredientTranslation(ingredientId, ingredient)) return;

	ta.commit();
}

QList<Ingredient> DB::getIngredients()
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "i.id, i.foodCategoryId, i.isLiquid, i.containsGluten, i.containsLactose, i18n.language, i18n.name "
	            "FROM "
	              "ingredients i, ingredients_i18n i18n "
	            "WHERE "
	               "i.id = i18n.ingredientId"
	            );

	QList<Ingredient> retval;
	if (!execQuery(query)) return retval;

	QHash<int, Ingredient> dummy;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(5).toString());

		Ingredient ingredient;
		if (dummy.contains(id)) {
			ingredient = dummy.value(id);
		} else {
			ingredient = Ingredient(query.value(2).toBool(), query.value(3).toBool(), query.value(4).toBool());
			ingredient.setFoodCategory(DB::getCategories(query.value(1).toInt()).first());
		}
		ingredient.updateName(lang, query.value(6).toString());

		dummy.insert(id, ingredient);
	}

	ta.commit();

	return dummy.values();
}

// portions

static int getPortionId(const Locale2String & names)
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "portionId "
	            "FROM "
	              "portions_i18n "
	            "WHERE "
	              "language = :language AND description = :description"
	            );

	int retval = -1;
	foreach (const QLocale & lang, names.keys()) {
		query.bindValue(":language",    lang.name());
		query.bindValue(":description", names.value(lang));
		if (!execQuery(query)) return retval;

		if (query.next()) {
			retval = query.value(0).toInt();
		}
		if (retval != -1) break;
	}
	ta.commit();

	return retval;
}

static bool addOrUpdatePortionTranslation(const int portionId, const Portion & portion)
{
	Transaction;

	QSqlQuery queryDescriptions(ta.db);
	queryDescriptions.prepare(
	        "INSERT INTO "
	          "portions_i18n "
	        "("
	          "portionId, language, description "
	        ") VALUES ("
	          ":portionId, :language, :description "
	        ") "
	        "ON DUPLICATE KEY UPDATE "
	          "description=VALUES(description) "
	            );

	queryDescriptions.bindValue(":portionId", portionId);

	Locale2String names = portion.getDescriptions();
	foreach(const QLocale & locale, names.keys()) {
		queryDescriptions.bindValue(":language",    locale.name());
		queryDescriptions.bindValue(":description", names.value(locale));
		execQuery(queryDescriptions);
	}

	return ta.commit();
}

void DB::addOrUpdatePortion(const Portion & portion)
{
	Transaction;

	int portionId = getPortionId(portion.getDescriptions());

	QString queryString;
	if (portionId == -1) {
		QSqlQuery query(ta.db);
		query.prepare("INSERT INTO portions () VALUES()");
		if(!execQuery(query)) return;
		portionId = query.lastInsertId().toUInt();
	}

	if (!addOrUpdatePortionTranslation(portionId, portion)) return;

	ta.commit();
}

QList<Portion> DB::getPortions()
{
	Transaction;

	QSqlQuery query(ta.db);
	query.prepare(
	            "SELECT "
	              "p.id, i18n.language, i18n.description "
	            "FROM "
	              "portions p, portions_i18n i18n "
	            "WHERE "
	               "p.id = i18n.portionId"
	            );

	QList<Portion> retval;
	if (!execQuery(query)) return retval;

	QHash<int, Portion> dummy;
	while (query.next()) {
		const int id = query.value(0).toInt();
		const QLocale lang = QLocale(query.value(1).toString());

		Portion portion;
		if (dummy.contains(id)) {
			portion = dummy.value(id);
		} else {
			portion = Portion();
		}
		portion.updateDescriptions(lang, query.value(2).toString());

		dummy.insert(id, portion);
	}

	ta.commit();

	return dummy.values();
}
