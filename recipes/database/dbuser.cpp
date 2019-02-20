#include "dbuser.h"

#include <recipes/common/user.h>
#include <recipes/database/database.h>
#include <recipes/database/password.h>

#include <QSqlQuery>

bool DB::updateUser(const User & user, const QString password)
{
	TRANSACTION(ta);
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
	query.bindValue(":passwordHash", Password::hashPassword(password));
	query.bindValue(":permissions",  uint(user.getPermissions()));
	query.bindValue(":isDeleted",    false);

	if (!Database::executeQuery(query)) return false;

	return ta.commit();
}

User DB::getUser(const QString & login)
{
	TRANSACTION(ta);
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
	if (!Database::executeQuery(query)) {
		return User();
	}
	if (!query.next()) {
		return User();
	}

	User user(
	            UserId(query.value(0).toInt()),
	            query.value(1).toString(),
	            Permissions(query.value(2).toUInt()),
	            query.value(3).toString(),
	            query.value(4).toString(),
	            query.value(5).toBool()
	            );

	return user;
}

bool DB::checkPassword(const QString &login, const QString &password)
{
	TRANSACTION(ta);
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
	if (!Database::executeQuery(query)) {
		return false;
	}
	if (!query.next()) {
		return false;
	}

	if (!Password::checkPassword(password, query.value(0).toByteArray())) {
		return false;
	}

	return true;
}

QList<User> DB::getAllUsers(const int & id)
{
	TRANSACTION(ta);

	QString queryStr =
	            "SELECT "
	                "id, login, permissions, firstName, lastName, isDeleted "
	            "FROM "
	                "users ";
	if (id != -1) {
		queryStr += " WHERE id = :id";
	}

	QSqlQuery query(ta.db);
	query.prepare(queryStr);

	if (id != -1) {
		query.bindValue(":id", id);
	}

	if (!Database::executeQuery(query)) return QList<User>();

	QList<User> retval;
	while (query.next()) {
		User user(
		            UserId(query.value(0).toInt()),
		            query.value(1).toString(),
		            Permissions(query.value(2).toUInt()),
		            query.value(3).toString(),
		            query.value(4).toString(),
		            query.value(5).toBool()
		            );

		retval << user;
	}

	if (!ta.commit()) return QList<User>();
	return retval;
}

UserId DB::getUserId(const QString & login)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	query.prepare(
	            "SELECT "
	                "id "
	            "FROM "
	                "users "
	            "WHERE "
	                "login = :login"
	            );
	query.bindValue(":login", login);
	if (!Database::executeQuery(query)) {
		return UserId();
	}
	if (!query.next()) {
		return UserId();
	}

	return UserId(query.value(0).toInt());
}
