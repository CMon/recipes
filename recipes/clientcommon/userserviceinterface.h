#pragma once

#include <QObject>

class RPCClient;
class User;

class UserServiceInterface : public QObject
{
	Q_OBJECT
	Q_PROPERTY(bool isLoggedIn READ loggedIn NOTIFY loggedInChanged)
public:
	UserServiceInterface(RPCClient * rpcClient, QObject *parent = nullptr);

	Q_INVOKABLE void login(const QString & username, const QString & password) const;
	bool loggedIn() { return isLoggedIn_; }

signals:
	void loggedInChanged();
	void loginError();

private slots:
	void logout();
	void handleMessageFromServer(const QVariantHash & map);

private:
	void handleCallbackResponse(const QString & signature, const std::string & string);
	void handleNotificationResponse(const QString & notification, const std::string & string);

private:
	void handleLogin(const User & user);

private:
	bool isLoggedIn_;
	RPCClient * rpcClient_;
};
