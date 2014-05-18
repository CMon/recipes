#include "userservice.h"

#include <cflib/util/log.h>
#include <cflib/crypt/util.h>
#include <database/dbuser.h>

USE_LOG(LogCat::User)

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
	logFunctionTrace

	if (!DB::checkPassword(login, password)) {
		logWarn("wrong passwort for login: %1", login);
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
		logWarn("permission not sufficient for user: %1", currentUsers_[clientId()].toString());
		return false;
	}

	if (!user.isValid()) {
		logWarn("invalid input chars in user: %1", user.toString());
		return false;
	}

	if (user.getLogin().isEmpty()) {
		logInfo("Could not add user, no login given");
		return false;
	}

	if (password.isEmpty()) {
		logInfo("no password given, adding random password");
		password = cflib::crypt::randomId().left(6);
	}

	return DB::updateUser(user, password);
}

QList<User> UserService::getUsers()
{
	logFunctionTrace

	if (!user_.hasPermission(Permission::Admin)) {
		logWarn("permission not sufficient for user: %1", currentUsers_[clientId()].toString());
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
