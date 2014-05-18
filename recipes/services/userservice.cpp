#include "userservice.h"

#include <common/log.h>
#include <cflib/crypt/util.h>
#include <database/dbuser.h>

UserService::UserService() :
    JSService(serializeTypeInfo().typeName)
{
}

UserService::~UserService()
{
	stopVerifyThread();
}

bool UserService::login(const QString &login, const QString &password, User &user)
{
	LOG_DEBUG_FUNCTION;

	if (!DB::checkPassword(login, password)) {
		LOG_WARN("wrong passwort for login: %1", login);
		return false;
	}

	user = DB::getUser(login);

	if (user.isNull()) return false;

	currentUsers_[clientId()] = user;

	return true;
}

bool UserService::logout()
{
	currentUsers_[clientId()] = User();
	return true;
}

bool UserService::addUser(const User & user, QString password)
{
	if (!user_.hasPermission(Permission::Admin)) {
		LOG_WARN("permission not sufficient for user: %1", currentUsers_[clientId()].toString());
		return false;
	}

	if (!user.isValid()) {
		LOG_WARN("invalid input chars in user: %1", user.toString());
		return false;
	}

	if (user.getLogin().isEmpty()) {
		LOG_INFO("Could not add user, no login given");
		return false;
	}

	if (password.isEmpty()) {
		LOG_INFO("no password given, adding random password");
		password = cflib::crypt::randomId().left(6);
	}

	return DB::updateUser(user, password);
}

QList<User> UserService::getUsers()
{
	LOG_DEBUG_FUNCTION;

	if (!user_.hasPermission(Permission::Admin)) {
		LOG_WARN("permission not sufficient for user: %1", currentUsers_[clientId()].toString());
		return QList<User>();
	}

	return DB::getAllUsers();
}

void UserService::clientExpired(uint clId)
{
	if (!verifyThreadCall(&UserService::clientExpired, clId)) return;
	currentUsers_.remove(clId);
}

void UserService::getCurrentUser(uint clId, User & user)
{
	if (!verifySyncedThreadCall(&UserService::getCurrentUser, clId, user)) return;
	user = currentUsers_[clId];
}

void UserService::preCallInit(const cflib::http::Request & request, uint clId)
{
	Q_UNUSED(request);
	user_ = currentUsers_.value(clId);
}
