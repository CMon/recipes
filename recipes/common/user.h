#pragma once

#include <recipes/common/permission.h>

#include <rpclib/common/types/types.h>
#include <cereal/cereal.hpp>

#include <QString>

class UserId
{
public:
	UserId() : id_(-1) {}
	UserId(const int id) : id_(id) {}

	QString toString() const { return QString::number(id_); }

	bool isNull() const { return id_ == -1; }
	bool isValid() const { return id_ > 0; }

	int toDatabaseValue() const { return id_; }

private:
	int id_;
};

class User
{
public:
	User();
	User(const QString & login, const Permissions & permissions, const QString & firstName, const QString & lastName, bool isDeleted = false);

	bool isNull() const;
	bool hasPermission(const Permissions::Permission & permission) const;
	bool isValidForServer() const;
	bool isValidForClient() const;

	QString toString() const;

	QString getLogin() const { return login_; }
	Permissions getPermissions() const { return permissions_; }
	QString getFirstName() const { return firstName_; }
	QString getLastName() const { return lastName_; }
	bool getIsDeleted() const { return isDeleted_; }

	bool operator ==(const User & rhs) const;
	bool operator !=(const User & rhs) const;

	template <class Archive>
	void serialize(Archive & ar) {
		ar(cereal::make_nvp("login", login_),
		   cereal::make_nvp("permissions", permissions_),
		   cereal::make_nvp("firstName", firstName_),
		   cereal::make_nvp("lastName", lastName_),
		   cereal::make_nvp("isDeleted", isDeleted_));
	}

	UserId getId() const;
	void setId(const UserId & id);

private:
	UserId id_; // do not serialize

private:
	QString login_;
	Permissions permissions_;
	QString firstName_;
	QString lastName_;
	bool    isDeleted_;
};
CEREAL_CLASS_VERSION(User, 1);
