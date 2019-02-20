#include "clientinfocache.h"

ClientInfoCache &ClientInfoCache::instance()
{
	static ClientInfoCache instance;
	return instance;
}

User ClientInfoCache::getUser(QWebSocket * socket) const
{
	return currentLoggedInUsers_.value(socket);
}

void ClientInfoCache::removeUser(QWebSocket * socket)
{
	currentLoggedInUsers_.remove(socket);
}

void ClientInfoCache::addUser(QWebSocket * socket, const User & user)
{
	currentLoggedInUsers_[socket] = user;
}
