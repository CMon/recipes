#include "userserviceinterface.h"

#include <recipes/common/user.h>
#include <recipes/clientcommon/logcategory.h>

#include <rpclib/client/rpcclient.h>
#include <cereal/archives/json.hpp>
#include <QJsonObject>
#include <QJsonDocument>

Q_LOGGING_CATEGORY(GUI_CLIENT, "clientcommon.services")

UserServiceInterface::UserServiceInterface(RPCClient * rpcClient, QObject * parent)
    : QObject(parent)
    , isLoggedIn_(false)
    , rpcClient_(rpcClient)
{
	connect(rpcClient_, &RPCClient::newMessageArrived, this, &UserServiceInterface::handleMessageFromServer);
	connect(rpcClient_, &RPCClient::disconnected, this, &UserServiceInterface::logout);
}

void UserServiceInterface::login(const QString & username, const QString & password) const
{
	if (username.isEmpty() || password.isEmpty()) {
		qCWarning(GUI_CLIENT) << "username and password have to be provided to login";
		return;
	}

	if (!rpcClient_->isConnected()) {
		qCWarning(GUI_CLIENT) << "not connected to any host, connect first";
		return;
	}

	QJsonObject loginData;
	loginData.insert("login", username);
	loginData.insert("password", password);

	rpcClient_->messageToServer("User UserService::login(QString, QString)", loginData);
}

void UserServiceInterface::logout()
{
	isLoggedIn_ = false;
	emit loggedInChanged();
}

void UserServiceInterface::handleMessageFromServer(const QVariantHash & map)
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

void UserServiceInterface::handleCallbackResponse(const QString & signature, const std::string & string)
{
	std::stringstream stream;
	stream.str(string);
	cereal::JSONInputArchive archive(stream);

	if (signature == "User UserService::login(QString, QString)") {
		User user;
		archive(user);
		handleLogin(user);
	}
}

void UserServiceInterface::handleNotificationResponse(const QString & notification, const std::string & string)
{
	std::stringstream stream;
	stream.str(string);
	cereal::JSONInputArchive archive(stream);

	Q_UNUSED(notification);
//	if (notification == "ChatService::newMessage(Message)") {
//	}
}

void UserServiceInterface::handleLogin(const User & user)
{
	if (!user.isValidForClient()) {
		isLoggedIn_ = false;
		emit loginError();
	} else {
		isLoggedIn_ = true;
		emit loggedInChanged();
	}
}
