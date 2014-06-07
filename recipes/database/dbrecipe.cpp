#include "dbrecipe.h"

#include <cflib/db/db.h>

#include <QLocale>
#include <QHash>

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

// units

static int getUnitId(const QLocale & language, const QString & abbreviation)
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
	query.bindValue(":language", language.name());
	query.bindValue(":abbreviation", abbreviation);

	int retval = -1;
	if (!execQuery(query)) return retval;

	if (query.next()) {
		retval = query.value(0).toInt();
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

	const Locale2String abbrev = unit.getAbbreviations();
	int unitId = -1;
	foreach (const QLocale & lang, abbrev.keys()) {
		unitId = getUnitId(lang, abbrev.value(lang));
		if (unitId != -1) break;
	}

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
