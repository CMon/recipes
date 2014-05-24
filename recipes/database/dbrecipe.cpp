#include "dbrecipe.h"

#include <cflib/db/db.h>

#include <QLocale>
#include <QHash>

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

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
	query.bindValue("language", language.bcp47Name());
	query.bindValue("abbreviation", abbreviation);

	if (!execQueryCommit(query) || !query.next()) return -1;

	return query.value(0).toInt();
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

	QString queryStr =
	        "INSERT INTO "
	            "units "
	        "("
	            "inGram, inMl"
	        ") VALUES ("
	            ":inGram, :inMl,"
	        ")";
	if (unitId != -1) {
		queryStr +
		        " WHERE id=:unitId "
		        "ON DUPLICATE KEY UPDATE "
		        "inGram=VALUES(inGram), "
		        "inMl=VALUES(inMl) ";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	query.bindValue(":inGram", unit.getGram());
	query.bindValue(":inMl",   unit.getMl());
	if (unitId != -1) {
		query.bindValue(":unitId", unitId);
	}
	if(!execQuery(query)) return;

	if (unitId != -1) {
		unitId = query.lastInsertId().toUInt();
	}

	if (!execQuery(query)) return;

	if (!addOrUpdateUnitTranslations(unitId, unit)) return;

	ta.commit();
}

bool DB::addOrUpdateUnitTranslations(const int unitId, const Unit & unit)
{
	Transaction;

	QSqlQuery queryAbbrev(ta.db);
	queryAbbrev.prepare(
	        "INSERT INTO "
	          "units "
	        "("
	          "unitId, language, abbreviation "
	        ") VALUES ("
	          ":unitId, :language, :abbreviation "
	        ") WHERE "
	          "unitId=:unitId "
	        "ON DUPLICATE KEY UPDATE "
	          "abbreviation=VALUES(abbreviation) "
	            );

	queryAbbrev.bindValue(":unitId", unitId);

	Locale2String abbrevs = unit.getAbbreviations();
	foreach(const QLocale & locale, abbrevs) {
		queryAbbrev.bindValue(":language",     locale.bcp47Name());
		queryAbbrev.bindValue(":abbreviation", abbrevs.value(locale));
		execQuery(queryAbbrev);
	}

	QSqlQuery queryName(ta.db);
	queryName.prepare(
	        "INSERT INTO "
	          "units "
	        "("
	          "unitId, language, completeName "
	        ") VALUES ("
	          ":unitId, :language, :completeName "
	        ") WHERE "
	          "unitId=:unitId "
	        "ON DUPLICATE KEY UPDATE "
	          "completeName=VALUES(completeName) "
	            );

	queryName.bindValue(":unitId", unitId);

	Locale2String names = unit.getCompleteNames();
	foreach(const QLocale & locale, names) {
		queryName.bindValue(":language",     locale.bcp47Name());
		queryName.bindValue(":completeName", names.value(locale));
		execQuery(queryName);
	}

	return ta.commit();
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
		const QString langStr = query.value(3).toString();

		Unit unit(query.value(1).toInt(), query.value(2).toInt());
		unit.updateAbbreviation(langStr, query.value(4).toString());
		unit.updateCompleteName(langStr, query.value(5).toString());

		dummy.insert(id, unit);
	}

	return dummy.values();
}
