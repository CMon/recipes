#include "userservice.h"

#include <recipes/database/dbuser.h>
#include <recipes/services/logCategory.h>
#include <recipes/servercommon/clientinfocache.h>

#include <cflib/crypt/util.h>

UserService::UserService() :
	RMIService(serializeTypeInfo().typeName)
{
}

UserService::~UserService()
{
	stopVerifyThread();
}

bool UserService::login(const QString &login, const QString &password, User &user)
{
	if (!DB::checkPassword(login, password)) {
		qCWarning(SERVICES) << "wrong passwort for login:" << login;
		return false;
	}

	user = DB::getUser(login);

	if (user.isNull()) return false;

	ClientInfoCache::instance().addUser(connId(), user);

	return true;
}

bool UserService::logout()
{
	ClientInfoCache::instance().removeUser(connId());
	return true;
}

bool UserService::addUser(const User & user, QString password)
{
	if (!currentUser_.hasPermission(Permission::Admin)) {
		qCWarning(SERVICES) << "permission not sufficient for user:" << currentUser_.toString();
		return false;
	}

	if (!user.isValid()) {
		qCWarning(SERVICES) << "invalid input chars in user:" << user.toString();
		return false;
	}

	if (user.getLogin().isEmpty()) {
		qCDebug(SERVICES) << "Could not add user, no login given";
		return false;
	}

	if (password.isEmpty()) {
		qCDebug(SERVICES) << "no password given, adding random password";
		password = cflib::crypt::randomId().left(6);
	}

	return DB::updateUser(user, password);
}

QList<User> UserService::getUsers()
{
	if (!currentUser_.hasPermission(Permission::Admin)) {
		qCWarning(SERVICES) << "permission not sufficient for user:" << currentUser_.toString();
		return QList<User>();
	}

	return DB::getAllUsers();
}

void UserService::connectionClosed(bool isLast)
{
	Q_UNUSED(isLast)
	ClientInfoCache::instance().removeUser(connId());
}

void UserService::preCallInit()
{
	currentUser_ = ClientInfoCache::instance().getUser(connId());
}
