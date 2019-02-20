#pragma once

#include <QSqlDatabase>
#include <QObject>
#include <QVariant> // query.bindValue will not work with it so include it here because otherwise i might search over and over again
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

#define TRANSACTION(varname) \
	Database::Transaction varname(Q_FUNC_INFO)

class Database : public QObject, public QSqlDatabase
{
	Q_OBJECT
public:
	enum Reason {
		NoRollback,
		ImplicitRollback,
		ExplicitRollback
	};

	class Transaction
	{
		Q_DISABLE_COPY(Transaction)
	public:
		enum State {
			InProgress,
			Committed,
			RolledBack
		};
		// instead of using this constructor use the define above
		Transaction(const char * funcName);
		~Transaction();

		Database & db;

		bool commit();
		void rollback();

	private:
		bool nested_;
		State state_;
		const char * funcName_;
	};

	static bool executeQuery(QSqlQuery & query);

public:
	static void setCredentials(const QString & dbName,
	                           const QString & user, const QString & password,
	                           const QString & hostname = "127.0.0.1", quint16 port = 0,
	                           const QString & driver = "QMYSQL");

	static Database & getThreadStoredDatabase();
	static void closeThreadStoredDatabase();

private:
	Database();

private:
	bool transactionInProgress_;
	bool transactionAlreadyCommitted_;
	Reason rollbackStatus_;
};
