#include "userserviceinterface.h"

#include <recipes/common/user.h>

#include <cereal/archives/json.hpp>
#include <QJsonDocument>
#include <QJsonObject>
#include <rpclib/client/rpcclient.h>
#include <rpclib/common/serializehelper.h>

#include <QLoggingCategory>
Q_DECLARE_LOGGING_CATEGORY(GUI_CLIENT_USERINTERFACE)
Q_LOGGING_CATEGORY(GUI_CLIENT_USERINTERFACE, "clientcommon.userservice")

UserServiceInterface::UserServiceInterface(RPCClient * rpcClient, QObject * parent)
    : QObject(parent)
    , isLoggedIn_(false)
    , rpcClient_(rpcClient)
{
	connect(rpcClient_, &RPCClient::newMessageArrived, this, &UserServiceInterface::handleMessageFromServer);
	connect(rpcClient_, &RPCClient::disconnected, this, &UserServiceInterface::onDisconnected);
}

UserServiceInterface::~UserServiceInterface()
{
	logout();
	onDisconnected();
}

void UserServiceInterface::login(const QString & username, const QString & password) const
{
	if (username.isEmpty() || password.isEmpty()) {
		qCWarning(GUI_CLIENT_USERINTERFACE) << "username and password have to be provided to login";
		return;
	}

	if (!rpcClient_->isConnected()) {
		qCWarning(GUI_CLIENT_USERINTERFACE) << "not connected to any host, connect first";
		return;
	}

	QJsonObject loginData;
	loginData.insert("login", username);
	loginData.insert("password", password);

	rpcClient_->messageToServer("User UserService::login(QString, QString)", loginData);
}

void UserServiceInterface::logout()
{
	rpcClient_->messageToServer("void UserService::logout()", QJsonValue());
}

void UserServiceInterface::addUser(const User & user, const QString & password)
{
	std::stringstream destStream;
	{
		cereal::JSONOutputArchive archive(destStream);
		archive(user);
		archive(password);
	}
	const std::string str = destStream.str();

	rpcClient_->messageToServer("bool UserService::addUser(User, QString)", QJsonDocument::fromJson(QByteArray::fromStdString(str)).object());
}

void UserServiceInterface::getAllUsers()
{
	rpcClient_->messageToServer("QList<User> UserService::getUsers()", QJsonValue());
}

bool UserServiceInterface::loggedIn()
{
	return isLoggedIn_;
}

void UserServiceInterface::onDisconnected()
{
	setLoggedIn(false);
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
	} else if (signature == "bool UserService::addUser(User, QString)") {
		bool added = false;
		archive(added);

		emit couldAddUser(added);
	} else if (signature == "QList<User> UserService::getUsers()") {
		QList<User> users;
		archive(users);

		emit listOfUsersReceived(users);
	}
}

void UserServiceInterface::handleNotificationResponse(const QString & notification, const std::string & string)
{
	std::stringstream stream;
	stream.str(string);
	cereal::JSONInputArchive archive(stream);

	Q_UNUSED(notification)
//	if (notification == "ChatService::newMessage(Message)") {
//	}
}

void UserServiceInterface::setLoggedIn(bool loggedIn)
{
	if (loggedIn == isLoggedIn_) {
		return;
	}

	isLoggedIn_ = false;
	emit loggedInChanged();
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
