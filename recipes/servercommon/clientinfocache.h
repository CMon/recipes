#pragma once

#include <recipes/common/user.h>

#include <QHash>

class QWebSocket;

class ClientInfoCache
{
public:
	static ClientInfoCache & instance();

public:
	ClientInfoCache(ClientInfoCache const&) = delete;
	void operator=(ClientInfoCache const&)  = delete;

	User getUser(QWebSocket * sendingSocket) const;
	void removeUser(QWebSocket * socket);
	void addUser(QWebSocket * socket, const User & user);

private:
	ClientInfoCache() {}

private:
	QHash<QWebSocket *, User> currentLoggedInUsers_;
};
