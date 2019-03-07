#pragma once

#include <QString>
#include <QSqlDatabase>

class QFile;

class Migrator
{
public:
	static const QString versionField;

	Migrator(const QString & dbName,
	         const QString & dbUser, const QString & dbPassword,
	         const QString & dbHostname = "127.0.0.1", quint16 dbPort = 0,
	         const QString & dbDriver = "QMYSQL",
	         const QString & connName = "migrator");

	bool update(const QString & schemeFilename);
	bool addTestData(const QString & testDataRef);
	bool addTestData(QFile testData);

private:
	bool isLatestVersion(const QString & schema);
	bool createSchemeVersionTable();

private:
	QString dbName_;
	QSqlDatabase db_;
	bool isInitialRun_;
	int oldDbVersion_;
	int newDbVersion_;
	int startPos_;
};
