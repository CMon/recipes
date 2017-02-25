#include "clientinfocache.h"

ClientInfoCache &ClientInfoCache::instance()
{
	static ClientInfoCache instance;
	return instance;
}

User ClientInfoCache::getUser(uint connId) const
{
	return currentLoggedInUsers_.value(connId);
}

void ClientInfoCache::removeUser(uint connId)
{
	currentLoggedInUsers_.remove(connId);
}

void ClientInfoCache::addUser(uint connId, const User & user)
{
	currentLoggedInUsers_[connId] = user;
}
