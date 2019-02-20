#include "database.h"

#include <QSqlDriver>
#include <QSqlQuery>
#include <QThread>
#include <QThreadStorage>
#include <QTimer>
#include <QDebug>

namespace {

QString dbName;
QString dbUser;
QString dbPassword;
QString dbHostname;
quint16 dbPort = 0;
QString dbDriver;

class DatabaseStorage {
	Q_DISABLE_COPY(DatabaseStorage)
public:
	Database * db;
	DatabaseStorage(Database * db) : db(db) {}
	~DatabaseStorage() {
		const QString connectionName = db->connectionName();

		// You can only remove a DB when there is no instance of QSqlDatabase anymore.
		db->close();
		delete db;
		QSqlDatabase::removeDatabase(connectionName);
	}
};

QThreadStorage<DatabaseStorage *> threadStoredDatabase;

QAtomicInt dbNumber;
QString createDbConnectionName()
{
	int i = dbNumber.fetchAndAddRelaxed(1);
	return "Database-" + QString::number(i);
}

}

Database::Database()
    : QSqlDatabase(QSqlDatabase::addDatabase(dbDriver, createDbConnectionName()))
    , transactionInProgress_(false)
    , transactionAlreadyCommitted_(false)
    , rollbackStatus_(NoRollback)
{
	if (!isValid()) {
		qCritical("cannot create Database driver");
		return;
	}
	if (dbName.isNull()) {
		qFatal("tried to use Database without calling setCredentials(...)");
		return;
	}

	// set connection settings
	if (!dbName.isEmpty()) setDatabaseName(dbName);
	setUserName(dbUser);
	setPassword(dbPassword);
	setHostName(dbHostname);
	if (dbPort != 0) setPort(dbPort);

	// open
	if (!open()) {
		qCritical("Cannot open Database");
		close();
		return;
	}

	// check for transactions
	if (!driver()->hasFeature(QSqlDriver::Transactions)) {
		qCritical("Database does not support transactions");
		close();
		return;
	}

	// check for last inserted id
	if (!driver()->hasFeature(QSqlDriver::LastInsertId)) {
		qCritical("Database does not support last inserted id");
		close();
		return;
	}
}

bool Database::executeQuery(QSqlQuery & query)
{
	bool queryAlreadyLogged = false;
	if (query.lastError().isValid()) {
		queryAlreadyLogged = true;
		qCritical() << "Query:" << query.lastQuery();
		qCritical() << query.lastError().number() << ":" << query.lastError().text();
	}
	if (!query.exec()) {
		if (!queryAlreadyLogged) {
			qWarning() << QString("Query: %1").arg(query.lastQuery());
		}
		qCritical() << QString("%1: %2").arg(query.lastError().number()).arg(query.lastError().text());
		return false;
	}
	return true;
}

void Database::setCredentials(const QString & databaseName,
                              const QString & user, const QString & password,
                              const QString & hostname, quint16 port,
                              const QString & driver)
{
	dbName = databaseName;
	dbUser = user;
	dbPassword = password;
	dbHostname = hostname;
	dbPort = port;
	dbDriver = driver;
}

Database & Database::getThreadStoredDatabase()
{
	if (!threadStoredDatabase.hasLocalData()) {
		threadStoredDatabase.setLocalData(new DatabaseStorage(new Database));
	}
	return *(threadStoredDatabase.localData()->db);
}

void Database::closeThreadStoredDatabase()
{
	if (threadStoredDatabase.hasLocalData()) threadStoredDatabase.setLocalData(0);
}

// ===========================================================================

Database::Transaction::Transaction(const char * funcName)
    :
      db(Database::getThreadStoredDatabase()),
      nested_(db.transactionInProgress_),
      state_(InProgress),
      funcName_(funcName)
{
	if (!nested_) {
		db.transactionInProgress_ = true;
		Q_ASSERT_X(db.transaction(), "Transaction", QString("could not start transaction from: >%1<").arg(funcName_).toLatin1());
	} else {
		if (db.transactionAlreadyCommitted_) {
			qFatal("started nested transaction on committed transaction in %s", funcName_);
		}
	}
}

Database::Transaction::~Transaction()
{
	if (state_ != Committed) {
		if (db.rollbackStatus_ != Database::ImplicitRollback) {
			if (state_ == RolledBack) db.rollbackStatus_ = Database::ExplicitRollback;
			else                      db.rollbackStatus_ = Database::ImplicitRollback;
		}

		if (state_ == RolledBack) {
			qDebug() << "explicit database transaction rollback initiated " << funcName_;
		} else {
			qCritical() << "implicit database transaction rollback initiated " << funcName_;
		}
	}

	if (nested_) {
		return;
	}

	if (db.rollbackStatus_ != Database::NoRollback) {
		if (db.rollbackStatus_ == Database::ExplicitRollback) {
			qDebug() << "explicitly rolling back transaction " << funcName_;
		} else {
			qCritical() << "implicitly rolling back transaction " << funcName_;
		}

		if (!db.rollback()) {
			qFatal("Database rollback failed in: %s", funcName_);
		}
		db.rollbackStatus_ = Database::NoRollback;
	}
	db.transactionInProgress_ = false;
	db.transactionAlreadyCommitted_ = false;
}

bool Database::Transaction::commit()
{
	Q_ASSERT_X(state_ == InProgress, "rollback", "transaction not in progress");
	state_ = Committed;

	if (db.rollbackStatus_ != Database::NoRollback) {
		qDebug() << "trying to commit already rolled back transaction: " << funcName_;
		return false;
	}

	if (nested_) {
		return true;
	}

	db.transactionAlreadyCommitted_ = true;
	if (!db.commit()) {
		qFatal("Database commit failed in: %s", funcName_);
	}

	return true;
}

void Database::Transaction::rollback()
{
	Q_ASSERT_X(state_ == InProgress, "rollback", "transaction not in progress");
	state_ = RolledBack;
	qDebug() << "rolled back transaction: " << funcName_;
}
