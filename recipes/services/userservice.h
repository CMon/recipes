#pragma once

#include <cflib/net/rmiservice.h>

#include <recipes/common/user.h>

class UserService : public QObject, public cflib::net::RMIService<QString>
{
	Q_OBJECT
	SERIALIZE_CLASS

public:
	UserService();
	~UserService();

rmi:
	bool login(const QString & login, const QString & password, User & user);
	bool logout();
	bool addUser(const User & login, QString password);
	QList<User> getUsers();

public:
	virtual void connectionClosed(bool isLast);

protected:
	virtual void preCallInit();

private:
	User currentUser_;
};
