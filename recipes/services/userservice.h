#pragma once

#include <recipes/common/user.h>

class RPCServer;
class QWebSocket;

class UserService : public QObject
{
public:
	UserService(QObject *parent = nullptr);

	void registerMethods(RPCServer * server);

public: // rpc methods
	User login(const QString & login, const QString & password, QWebSocket * sendingSocket);
	void logout(QWebSocket * client);
	bool addUser(User login, QString password, QWebSocket * sendingSocket);
	QList<User> getUsers(QWebSocket * sendingSocket);

private slots:
	void connectionClosed(QWebSocket * client);
};
