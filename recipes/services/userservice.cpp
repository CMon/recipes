#include "userservice.h"

#include <recipes/database/dbuser.h>
#include <recipes/servercommon/clientinfocache.h>

#include <cereal/archives/json.hpp>
#include <rpclib/common/serializehelper.h>
#include <rpclib/common/types/qlistcereal.h>
#include <rpclib/server/rpcserver.h>

#include <QJsonObject>
#include <QUuid>
#include <QWebSocket>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(USER_SERVICES)
Q_LOGGING_CATEGORY(USER_SERVICES, "recipe.services.user")

UserService::UserService(QObject * parent)
    : QObject (parent)
{
}

void UserService::registerMethods(RPCServer * server)
{
	connect(server, &RPCServer::clientDisconnected, this, &UserService::logout);

	server->addCallback("User UserService::login(QString, QString)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		QString username;
		QString password;
		cereal::JSONInputArchive archive(stream);
		archive(username);
		archive(password);

		const User user = login(username, password, sendingSocket);

		QJsonValue retval;
		CEREAL_2_DEST(retval, "units", user);
		return retval;
	}, "Login a user to the system" );

	server->addCallback("void UserService::logout()", [=] (QWebSocket * sendingSocket) {
		logout(sendingSocket);
		return QJsonObject();
	}, "logout the current user. This does not disconnect the session." );

	server->addCallback("bool UserService::addUser(User, QString)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		User user;
		QString password;
		cereal::JSONInputArchive archive(stream);
		archive(user);
		archive(password);

		QJsonObject ret;
		ret.insert("added", addUser(user, password, sendingSocket));
		return ret;
	}, "Add a user to the system" );

	server->addCallback("QList<User> UserService::getUsers()", [=] (QWebSocket * sendingSocket) {
		QJsonValue retval;
		CEREAL_2_DEST(retval, "units", getUsers(sendingSocket));

		return retval;
	}, "Get a list of users" );
}

User UserService::login(const QString &login, const QString &password, QWebSocket * sendingSocket)
{
	if (!DB::checkPassword(login, password)) {
		qCWarning(USER_SERVICES) << "wrong passwort for login:" << login;
		return User();
	}

	const User user = DB::getUser(login);

	if (user.isNull()) return user;

	qCInfo(USER_SERVICES) << "User" << login << "successfully logged in";
	ClientInfoCache::instance().addUser(sendingSocket, user);

	return user;
}

void UserService::logout(QWebSocket * client)
{
	ClientInfoCache::instance().removeUser(client);
	client->close(QWebSocketProtocol::CloseCodeGoingAway);
}

bool UserService::addUser(User user, QString password, QWebSocket * sendingSocket)
{
	const User currentUser = ClientInfoCache::instance().getUser(sendingSocket);

	if (!currentUser.hasPermission(Permissions::Administrator)) {
		qCWarning(USER_SERVICES) << "permission not sufficient for user:" << currentUser.toString();
		return false;
	}

	if (!user.isValidForClient()) {
		qCWarning(USER_SERVICES) << "invalid input chars in user:" << user.toString();
		return false;
	}

	if (user.getLogin().isEmpty()) {
		qCDebug(USER_SERVICES) << "Could not add user, no login given";
		return false;
	}

	if (password.isEmpty()) {
		qCDebug(USER_SERVICES) << "no password given, adding random password";
		password = QUuid::createUuid().toString().left(8);
	}

	return DB::addOrUpdateUser(user, password);
}

QList<User> UserService::getUsers(QWebSocket * sendingSocket)
{
	const User currentUser = ClientInfoCache::instance().getUser(sendingSocket);

	if (!currentUser.hasPermission(Permissions::Administrator)) {
		qCWarning(USER_SERVICES) << "permission not sufficient for user:" << currentUser.toString();
		return QList<User>();
	}

	return DB::getAllUsers();
}
