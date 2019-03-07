#include "migrator.h"

#include <recipes/database/database.h>

#include <QString>
#include <QFile>
#include <QDebug>
#include <QRegExp>
#include <QStringList>

const QString Migrator::versionField = "version";

Migrator::Migrator(const QString & dbName,
                   const QString & dbUser, const QString & dbPassword,
                   const QString & dbHostname, quint16 dbPort,
                   const QString & dbDriver, const QString & connName)
    : dbName_(dbName),
      isInitialRun_(false),
      oldDbVersion_(0),
      newDbVersion_(0),
      startPos_(-1)
{
	if (dbName.isNull()) {
		qDebug() << "database name is empty but shoud be set";
		return;
	}

	// set the database parameters
	Database::setCredentials(dbName, dbUser, dbPassword, dbHostname, dbPort, dbDriver);

	db_ = QSqlDatabase::addDatabase(dbDriver, connName);
	db_.setUserName(dbUser);
	db_.setPassword(dbPassword);
	db_.setHostName(dbHostname);
	if (dbPort != 0) db_.setPort(dbPort);

	isInitialRun_ = false;

	// try to open the database
	db_.setDatabaseName(dbName);
	if (!db_.open()) {
		isInitialRun_ = true;

		db_.setDatabaseName(QString());
		if (!db_.open()) {
			qWarning() << "could not open empty database:" << db_.lastError().text();
			return;
		}

		qDebug() << "creating and initializing database:" << dbName;
		if (!db_.exec("CREATE DATABASE " + dbName + ";").isActive()) {
			qCritical() << "could not create database:" << dbName;
			db_.close();
			return;
		}

		if (!db_.exec("USE " + dbName + ";").isActive()) {
			qCritical() << "could not use database:" << dbName;
			db_.close();
			return;
		}

		if (!createSchemeVersionTable()) {
			return;
		}

		if (!db_.exec("INSERT INTO scheme_version (" + versionField + ") VALUES (0);").isActive()) {
			qCritical() << "could not initialize table scheme_version";
			db_.close();
			return;
		}
	}
}

bool Migrator::isLatestVersion(const QString & schema)
{
	if(!db_.isOpen()) qCritical() << "Database not open";

	// get current version
	QSqlQuery query = db_.exec("SELECT `" + versionField + "` FROM scheme_version LIMIT 1;");

	if (!query.next()) {
		if (!createSchemeVersionTable()) {
			return false;
		}
		oldDbVersion_ = -1;
	} else {
		oldDbVersion_ = query.value(0).toInt();
	}

	const QRegExp schemaRx("-- scheme version[^\\d]*(\\d+)");
	newDbVersion_ = 0;
	int startPos = -1;
	int pos = schemaRx.indexIn(schema);
	while (pos != -1) {
		newDbVersion_ = schemaRx.cap(1).toInt();
		if (startPos == -1 && newDbVersion_ > oldDbVersion_) {
			startPos = pos;
		}
		pos = schemaRx.indexIn(schema, pos + schemaRx.matchedLength());
	}
	startPos_ = startPos;

	if(newDbVersion_ < oldDbVersion_) {
		qCritical() << "WARNING: Database is newer than the given sql structure!";
	}

	if (startPos_ == -1) {
		qDebug() << "database is up to date at version" << oldDbVersion_;
		return true;
	}
	return false;
}

bool Migrator::createSchemeVersionTable()
{
	if (!db_.exec("CREATE TABLE `scheme_version` ( `" + versionField + "` INTEGER unsigned NOT NULL DEFAULT 0);").isActive()) {
		qCritical() << "could not create table scheme_version";
		db_.close();
		return false;
	}
	return true;
}

bool Migrator::update(const QString & schemeFilename)
{
	QFile schemeFile(schemeFilename);

	if (!schemeFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Could not open scheme file:" << schemeFile.fileName();
		return false;
	}

	const QString schema = QString::fromUtf8(schemeFile.readAll());

	if(isLatestVersion(schema)) return true;

	foreach (QString queryStr, schema.mid(startPos_).remove(QRegExp("--[^\n]*")).split(';'))
	{
		queryStr = queryStr.trimmed();
		if (queryStr.isEmpty()) continue;

		db_.exec(queryStr);
		if (db_.lastError().isValid()) {
			qCritical() << "could not update DB schema of database:" << dbName_ << ":" << db_.lastError().text();
			qCritical() << "query was:" << queryStr;
			return false;
		}
	}

	// update version number
	db_.exec(QString("UPDATE scheme_version SET `%1`=%2;").arg(versionField).arg(newDbVersion_));
	if (db_.lastError().isValid()) {
		qCritical() << "could not update database schema version of database:" << dbName_ << ":" << db_.lastError().text() << "continuing";
	}

	qDebug() << "updated database schema from version" << oldDbVersion_ << "to version" << newDbVersion_;
	return true;
}

bool Migrator::addTestData(const QString & testDataRef)
{
	if (isInitialRun_) {
		QString testData = testDataRef;
		qDebug("inserting testdata into database");

		foreach (QString queryStr, testData.remove(QRegExp("--[^\n]*")).split(';')) {
			queryStr = queryStr.trimmed();
			if (queryStr.isEmpty()) continue;
			db_.exec(queryStr);
			if (db_.lastError().isValid()) {
				qCritical() << "could not add testdata";
				qCritical() << "query was:" << queryStr;
				return false;
			}
		}
	} else {
		qWarning() << "testdata can only be added on initial run";
		return false;
	}

	return true;
}

bool Migrator::addTestData(QFile testData)
{
	if (!testData.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qWarning() << "Could not open testdata file:" << testData.fileName();
		return false;
	}

	const bool ok = addTestData(testData.readAll());
	testData.close();
	return ok;
}
