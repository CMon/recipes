#include "dbuser.h"

#include <common/user.h>

#include <cflib/crypt/util.h>
#include <cflib/db/db.h>

#include <QSqlQuery>

USE_LOG(LogCat::Db) // needed by Transaction and other db methods from cflib

bool DB::updateUser(const User & user, const QString password)
{
	Transaction;
	QSqlQuery query(ta.db);

	query.prepare(
	            "INSERT INTO "
	                "users "
	            "("
	                "login, firstName, lastName, passwordHash, permissions, isDeleted"
	            ") VALUES ("
	                ":login, :firstName, :lastName, :passwordHash, :permissions, :isDeleted"
	            ")"
	            );
	query.bindValue(":login",        user.getLogin());
	query.bindValue(":firstName",    user.getFirstName());
	query.bindValue(":lastName",     user.getLastName());
	query.bindValue(":passwordHash", cflib::crypt::hashPassword(password));
	query.bindValue(":permissions",  (uint)user.getPermissions());
	query.bindValue(":isDeleted",    false);

	if (!execQuery(query)) return false;

	return ta.commit();
}

User DB::getUser(const QString & login)
{
	Transaction;
	QSqlQuery query(ta.db);

	query.prepare(
	            "SELECT "
	                "id, login, permissions, firstName, lastName, isDeleted "
	            "FROM "
	                "users "
	            "WHERE "
	                "login = :login"
	            );
	query.bindValue(":login", login);
	if (!execQueryCommit(query) || !query.next()) return User();

	User user(
	            UserId(query.value(0).toInt()),
	            query.value(1).toString(),
	            Permission(query.value(2).toUInt()),
	            query.value(3).toString(),
	            query.value(4).toString(),
	            query.value(5).toBool()
	            );

	return user;
}

bool DB::checkPassword(const QString &login, const QString &password)
{
	Transaction;
	QSqlQuery query(ta.db);

	query.prepare(
	            "SELECT "
	                "passwordHash "
	            "FROM "
	                "users "
	            "WHERE "
	                "login = :login"
	            );
	query.bindValue(":login", login);
	if (!execQueryCommit(query) || !query.next()) return false;

	if (!cflib::crypt::checkPassword(password, query.value(0).toByteArray())) {
		return false;
	}

	return true;
}

QList<User> DB::getAllUsers(const int & id)
{
	Transaction;

	QString queryStr =
	            "SELECT "
	                "id, login, permissions, firstName, lastName, isDeleted "
	            "FROM "
	                "users ";
	if (id != -1) {
		queryStr += " AND id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	if (!execQuery(query)) return QList<User>();

	QList<User> retval;
	while (query.next()) {
		User user(
		            UserId(query.value(0).toInt()),
		            query.value(1).toString(),
		            Permission(query.value(2).toUInt()),
		            query.value(3).toString(),
		            query.value(4).toString(),
		            query.value(5).toBool()
		            );

		retval << user;
	}

	if (!ta.commit()) return QList<User>();
	return retval;
}
