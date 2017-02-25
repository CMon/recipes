#pragma once

#include <recipes/common/user.h>

class ClientInfoCache
{
public:
	static ClientInfoCache & instance();

public:
	ClientInfoCache(ClientInfoCache const&) = delete;
	void operator=(ClientInfoCache const&)  = delete;

	User getUser(uint connId) const;
	void removeUser(uint connId);
	void addUser(uint connId, const User & user);

private:
	ClientInfoCache() {}

private:
	QHash<uint, User> currentLoggedInUsers_;
};
