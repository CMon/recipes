#pragma once

#include <cflib/serialize/serialize.h>
#include <QString>
#include <common/permission.h>

class UserId
{
public:
	UserId() : id_(-1) {}
	UserId(const int id) : id_(id) {}

	QString toString() const { return QString::number(id_); }

	bool isNull() const { return id_ == -1; }
	bool isValid() const { return id_ > 0; }

private:
	int id_;
};

class User
{
	SERIALIZE_CLASS

public:
	User();
	User(const UserId & id, const QString & login, const Permissions & permissions, const QString & firstName, const QString & lastName, bool isDeleted);

	bool isNull() const;
	bool hasPermission(const Permission & permission) const;
	bool isValid() const;

	QString toString() const;

	QString getLogin() const { return login_; }
	Permissions getPermissions() const { return permissions_; }
	QString getFirstName() const { return firstName_; }
	QString getLastName() const { return lastName_; }
	bool getIsDeleted() const { return isDeleted_; }

	bool operator ==(const User & rhs) const;
	bool operator !=(const User & rhs) const;

private:
	UserId id_;

private serialized:
	QString login_;
	Permissions permissions_;
	QString firstName_;
	QString lastName_;
	bool    isDeleted_;
};
