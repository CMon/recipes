#include "adminserviceinterface.h"

#include <rpclib/client/rpcclient.h>
#include <QJsonObject>
#include <QJsonDocument>
#include <rpclib/common/serializehelper.h>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(GUI_CLIENT_ADMININTERFACE)
Q_LOGGING_CATEGORY(GUI_CLIENT_ADMININTERFACE, "clientcommon.adminservice")

AdminServiceInterface::AdminServiceInterface(RPCClient * rpcClient, QObject *parent)
    : QObject(parent)
    , rpcClient_(rpcClient)
{
	connect(rpcClient_, &RPCClient::newMessageArrived, this, &AdminServiceInterface::handleMessageFromServer);
}

void AdminServiceInterface::getPermissions() const
{
	rpcClient_->messageToServer("QList<PermissionData> AdminService::getPermissions()", QJsonValue());
}

void AdminServiceInterface::addPermission(const QString & techName, const Locale2String & title, const Locale2String & description) const
{
	if (techName.isEmpty()) {
		qCWarning(GUI_CLIENT_ADMININTERFACE) << "techname needs to be filled";
		return;
	}

	std::stringstream destStream;
	{
		cereal::JSONOutputArchive archive(destStream);
		archive(techName);
		archive(title);
		archive(description);
	}
	const std::string str = destStream.str();

	rpcClient_->messageToServer("bool AdminService::addPermission(QString, Locale2String, Locale2String)", QJsonDocument::fromJson(QByteArray::fromStdString(str)).object());
}

void AdminServiceInterface::setPermissionTitle(const int permissionId, const Locale2String & title) const
{
	if (permissionId < 1) {
		qCWarning(GUI_CLIENT_ADMININTERFACE) << "Invalid permission id given";
		return;
	}

	std::stringstream destStream;
	{
		cereal::JSONOutputArchive archive(destStream);
		archive(permissionId);
		archive(title);
	}
	const std::string str = destStream.str();

	rpcClient_->messageToServer("bool AdminService::setPermissionTitle(int, Locale2String)", QJsonDocument::fromJson(QByteArray::fromStdString(str)).object());
}

void AdminServiceInterface::setPermissionDescription(const int permissionId, const Locale2String & description) const
{
	if (permissionId < 1) {
		qCWarning(GUI_CLIENT_ADMININTERFACE) << "Invalid permission id given";
		return;
	}

	std::stringstream destStream;
	{
		cereal::JSONOutputArchive archive(destStream);
		archive(permissionId);
		archive(description);
	}
	const std::string str = destStream.str();

	rpcClient_->messageToServer("bool AdminService::setPermissionDescription(int, Locale2String)", QJsonDocument::fromJson(QByteArray::fromStdString(str)).object());
}

QList<PermissionData> AdminServiceInterface::getCachedPermissions() const
{
	return permissions_;
}

void AdminServiceInterface::handleMessageFromServer(const QVariantHash & map)
{
	const QJsonObject message = QJsonObject::fromVariantHash(map);

	if (message.contains("signature")) {
		const std::string str = QJsonDocument(message.value("data").toObject()).toJson().toStdString();
		handleCallbackResponse(message.value("signature").toString(), str);
	}

	if (message.contains("notification")) {
		const std::string str = QJsonDocument(message.value("data").toObject()).toJson().toStdString();
		handleNotificationResponse(message.value("notification").toString(), str);
	}
}

void AdminServiceInterface::handleCallbackResponse(const QString & signature, const std::string & string)
{
	std::stringstream stream;
	stream.str(string);
	cereal::JSONInputArchive archive(stream);

	if (signature == "QList<PermissionData> AdminService::getPermissions()") {
		permissions_.clear();
		archive(permissions_);
		if (!permissions_.isEmpty()) {
			// there are always permissions, so if its empty i am not allowed to get them
			emit permissionsUpdated();
		}
	} else if (signature == "bool AdminService::addPermission(QString, Locale2String, Locale2String)") {
		bool success;
		archive(success);
		emit addPermissionSuccesful(success);
	} else if (signature == "bool AdminService::setPermissionTitle(int, Locale2String)") {
		bool success;
		archive(success);
		emit addPermissionTitleSuccesful(success);
	} else if (signature == "bool AdminService::setPermissionDescription(int, Locale2String)") {
		bool success;
		archive(success);
		emit addPermissionDescriptionSuccesful(success);
	}
}

void AdminServiceInterface::handleNotificationResponse(const QString & notification, const std::string & string)
{
	std::stringstream stream;
	stream.str(string);
	cereal::JSONInputArchive archive(stream);

	Q_UNUSED(notification)
//	if (notification == "ChatService::newMessage(Message)") {
//	}
}
