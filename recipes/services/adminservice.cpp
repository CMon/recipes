#include "adminservice.h"

#include <recipes/database/dbpermission.h>
#include <recipes/servercommon/clientinfocache.h>

#include <rpclib/common/serializehelper.h>
#include <rpclib/common/types/qlistcereal.h>
#include <rpclib/server/rpcserver.h>

#include <QJsonObject>
#include <QWebSocket>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(ADMIN_SERVICES)
Q_LOGGING_CATEGORY(ADMIN_SERVICES, "recipe.services.admin")

AdminService::AdminService(QObject * parent)
    : QObject (parent)
{
}

void AdminService::registerMethods(RPCServer * server)
{
	server->addCallback("QList<PermissionData> AdminService::getPermissions()", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		QJsonValue retval;
		CEREAL_2_DEST(retval, "permissions", getPermissions(sendingSocket));
		return retval;
	}, "Get a list of available permissions." );

	server->addCallback("bool AdminService::addPermission(QString, Locale2String, Locale2String)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		QString techName;
		Locale2String title;
		Locale2String description;
		cereal::JSONInputArchive archive(stream);
		archive(techName);
		archive(title);
		archive(description);

		const bool added = addPermission(sendingSocket, techName, title, description);

		QJsonValue retval;
		CEREAL_2_DEST(retval, "added", added);
		return retval;
	}, "Add a new permission." );

	server->addCallback("bool AdminService::setPermissionTitle(int, Locale2String)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		int permissionId;
		Locale2String title;
		cereal::JSONInputArchive archive(stream);
		archive(permissionId);
		archive(title);

		const bool added = setPermissionTitle(sendingSocket, permissionId, title);

		QJsonValue retval;
		CEREAL_2_DEST(retval, "added", added);
		return retval;
	}, "Set a title of the permission, if the locale exists then it is overwritten." );

	server->addCallback("bool AdminService::setPermissionDescription(int, Locale2String)", [=] (std::stringstream & stream, QWebSocket * sendingSocket) {
		int permissionId;
		Locale2String description;
		cereal::JSONInputArchive archive(stream);
		archive(permissionId);
		archive(description);

		const bool added = setPermissionDescription(sendingSocket, permissionId, description);

		QJsonValue retval;
		CEREAL_2_DEST(retval, "added", added);
		return retval;
	}, "Set a description of the permission, if the locale exists then it is overwritten." );
}

QList<PermissionData> AdminService::getPermissions(QWebSocket * sendingSocket)
{
	const User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	if (!currentUser.hasPermission(Permissions::Administrator)) {
		qCWarning(ADMIN_SERVICES) << "permission not sufficient for user:" << currentUser.toString();
		return QList<PermissionData>();
	}

	return DB::getPermissions();
}

bool AdminService::addPermission(QWebSocket * sendingSocket, const QString & techName, const Locale2String & title, const Locale2String & description)
{
	const User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	if (!currentUser.hasPermission(Permissions::Administrator)) {
		qCWarning(ADMIN_SERVICES) << "permission not sufficient for user:" << currentUser.toString();
		return false;
	}

	if (techName.isEmpty()) {
		qCWarning(ADMIN_SERVICES) << "techname of new permission has to be set";
		return false;
	}

	const int permissionId = DB::addPermission(techName);
	if (permissionId < 0) {
		qCWarning(ADMIN_SERVICES) << "An error occured on adding a new permission";
		return false;
	}

	if (!DB::setPermissionTitle(permissionId, title)) {
		qCWarning(ADMIN_SERVICES) << "An error occured on adding a title to permission with id:" << permissionId;
		return false;
	}

	if (!DB::setPermissionDescription(permissionId, description)) {
		qCWarning(ADMIN_SERVICES) << "An error occured on adding a description to permission with id:" << permissionId;
		return false;
	}

	return true;
}

bool AdminService::setPermissionTitle(QWebSocket * sendingSocket, const int permissionId, const Locale2String & title)
{
	const User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	if (!currentUser.hasPermission(Permissions::Administrator)) {
		qCWarning(ADMIN_SERVICES) << "permission not sufficient for user:" << currentUser.toString();
		return false;
	}

	if (!DB::setPermissionTitle(permissionId, title)) {
		qCWarning(ADMIN_SERVICES) << "An error occured on adding a title to permission with id:" << permissionId;
		return false;
	}

	return true;
}

bool AdminService::setPermissionDescription(QWebSocket * sendingSocket, const int permissionId, const Locale2String & description)
{
	const User currentUser = ClientInfoCache::instance().getUser(sendingSocket);
	if (!currentUser.hasPermission(Permissions::Administrator)) {
		qCWarning(ADMIN_SERVICES) << "permission not sufficient for user:" << currentUser.toString();
		return false;
	}

	if (!DB::setPermissionDescription(permissionId, description)) {
		qCWarning(ADMIN_SERVICES) << "An error occured on adding a description to permission with id:" << permissionId;
		return false;
	}

	return true;
}
