#include "dbuser.h"

#include <recipes/common/user.h>
#include <recipes/database/database.h>
#include <recipes/database/password.h>

#include <QSqlQuery>

bool updatePermissions(const UserId & userId, const Permissions & perms)
{
	TRANSACTION(ta);
	QSqlQuery removeQuery(ta.db);
	removeQuery.prepare(
	            "DELETE FROM "
	                "userPermission "
	            "WHERE "
	                "userId = :userId"
	            );
	removeQuery.bindValue(":userId", userId.toDatabaseValue());
	Database::executeQuery(removeQuery);

	QSqlQuery query(ta.db);
	query.prepare(
	            "INSERT INTO "
	                "userPermission "
	            "("
	                "userId, permissionId"
	            ") VALUES ("
	                ":userId, :permissionId"
	            ")"
	            );

	for (int perm: perms) {
		query.bindValue(":userId",       userId.toDatabaseValue());
		query.bindValue(":permissionId", perm);

		if (!Database::executeQuery(query)) return false;
	}

	return ta.commit();
}

bool getPermissions(const UserId & userId, Permissions & perms)
{
	TRANSACTION(ta);
	perms = Permissions();
	QSqlQuery query(ta.db);

	query.prepare("SELECT permissionId from userPermission WHERE userId = :userId");
	query.bindValue(":userId", userId.toDatabaseValue());

	if (!Database::executeQuery(query)) return false;

	while (query.next()) {
		perms.addPermission(Permissions::Permission(query.value(0).toInt()));
	}
	if (!ta.commit()) return false;
	return true;
}

bool addUser(const User & user, QString password)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	if (password.isEmpty()) {
		password = Password::getRandomPassword();
	}

	qInfo() << "Adding new user:" << user.getLogin();
	query.prepare(
	            "INSERT INTO "
	                "users "
	            "("
	                "login, firstName, lastName, passwordHash, passwordCrypto, isDeleted"
	            ") VALUES ("
	                ":login, :firstName, :lastName, :passwordHash, :passwordCrypto, :isDeleted"
	            ")"
	            );

	query.bindValue(":login",        user.getLogin());
	query.bindValue(":firstName",    user.getFirstName());
	query.bindValue(":lastName",     user.getLastName());
	QString usedCrypto;
	query.bindValue(":passwordHash",   Password::hashPassword(password, usedCrypto));
	query.bindValue(":passwordCrypto", usedCrypto);
	query.bindValue(":isDeleted",    false);

	if (!Database::executeQuery(query)) return false;
	return ta.commit();
}

bool updateUser(const User & user, const QString & password)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	QString queryStr =
	        "UPDATE users "
	        "WHERE id = :id "
	        "SET "
	        "login            = :login"
	        ", firstName      = :firstName"
	        ", lastName       = :lastName"
	        ". isDeleted      = :isDeleted";

	if (!password.isEmpty()) {
		queryStr +=
		        ", passwordHash   = :passwordHash"
		        ". passwordCrypto = :passwordCrypto";
	}

	query.prepare(queryStr);

	query.bindValue(":id",        user.getId().toDatabaseValue());
	query.bindValue(":login",     user.getLogin());
	query.bindValue(":firstName", user.getFirstName());
	query.bindValue(":lastName",  user.getLastName());
	if (!password.isEmpty()) {
		QString usedCrypto;
		query.bindValue(":passwordHash",   Password::hashPassword(password, usedCrypto));
		query.bindValue(":passwordCrypto", usedCrypto);
	}
	query.bindValue(":isDeleted", false);

	return ta.commit();
}

bool DB::addOrUpdateUser(User & user, const QString & password)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	UserId userId = DB::getUserId(user.getLogin());
	if (!userId.isValid()) {
		if (!addUser(user, password)) return false;
		userId = DB::getUserId(user.getLogin());
		user.setId(userId);
	} else {
		if (!updateUser(user, password)) return false;
	}

	if (!userId.isValid()) {
		qInfo() << "Some error occured during changing the user with the login:" << user.getLogin();
		return false;
	}

	if (!updatePermissions(userId, user.getPermissions())) return false;

	return ta.commit();
}

bool DB::resetPassword(const UserId & userId)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);
	const QString password = Password::getRandomPassword();

	query.prepare(
	            "UPDATE users "
		        "WHERE id = :id "
		        "SET "
		        "passwordHash     = :passwordHash"
		        ", passwordCrypto = :passwordCrypto"
	            );

	QString usedCrypto;
	query.bindValue(":id", userId.toDatabaseValue());
	query.bindValue(":passwordHash",   Password::hashPassword(password, usedCrypto));
	query.bindValue(":passwordCrypto", usedCrypto);

	if (!Database::executeQuery(query)) return false;

	return ta.commit();
}

User DB::getUser(const QString & login)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	query.prepare(
	            "SELECT "
	                "id, login, firstName, lastName, isDeleted "
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

	const UserId userId = query.value(0).toInt();

	Permissions perms;
	if (!getPermissions(userId, perms)) return User();

	User user(query.value(1).toString(),
	          perms,
	          query.value(2).toString(),
	          query.value(3).toString(),
	          query.value(4).toBool()
	          );
	user.setId(userId);

	if (!ta.commit()) return User();

	return user;
}

bool DB::checkPassword(const QString &login, const QString &password)
{
	TRANSACTION(ta);
	QSqlQuery query(ta.db);

	query.prepare(
	            "SELECT "
	                "passwordHash, passwordCrypto "
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
	if (!ta.commit()) return false;

	if (!Password::checkPassword(password, query.value(0).toByteArray(), query.value(1).toString())) {
		return false;
	}

	return true;
}

QList<User> DB::getAllUsers(const int & id)
{
	TRANSACTION(ta);

	QString queryStr =
	            "SELECT "
	                "id, login, firstName, lastName, isDeleted "
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
		const UserId userId = query.value(0).toInt();

		Permissions perms;
		if (!getPermissions(userId, perms)) continue;

		User user(query.value(1).toString(),
		          perms,
		          query.value(2).toString(),
		          query.value(3).toString(),
		          query.value(4).toBool()
		          );
		user.setId(userId);

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
		ta.commit();
		return UserId();
	}

	const UserId retval = UserId(query.value(0).toInt());
	ta.commit();
	return retval;
}
