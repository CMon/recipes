#include "usersmodel.h"

#include <recipes/clientcommon/userserviceinterface.h>

UsersModel::UsersModel(RPCClient * rpcClient, UserServiceInterface * userService, QObject *parent)
    : QAbstractListModel(parent)
    , rpcClient_(rpcClient)
    , userService_(userService)
{
	connect(userService_, &UserServiceInterface::listOfUsersReceived, this, &UsersModel::onUpdateUserList);
}

void UsersModel::update()
{
	userService_->getAllUsers();
}

int UsersModel::rowCount(const QModelIndex &) const
{
	return users_.size();
}

QVariant UsersModel::data(const QModelIndex & index, int role) const
{
	if (index.row() < 0 || index.row() >= rowCount(index)) return QVariant();
	const int row = index.row();

	switch (role) {
		case LoginRole:       return users_.at(row).getLogin();
		case FirstNameRole:   return users_.at(row).getFirstName();
		case LastNameRole:    return users_.at(row).getLogin();
		case PermissionsRole: {
//			users_.at(row).getPermissions(), then add all of them in the ui's language together into one list
			QString permissions = "todo";
			return permissions;
		}
		case IsDeletedRole:   return users_.at(row).getIsDeleted();
	}

	return QVariant();
}

QHash<int, QByteArray> UsersModel::roleNames() const
{
	QHash<int, QByteArray> retval;
	retval[LoginRole]       = "login";
	retval[FirstNameRole]   = "firstName";
	retval[LastNameRole]    = "lastName";
	retval[PermissionsRole] = "permissions";
	retval[IsDeletedRole]   = "isDeleted";
	return retval;
}

void UsersModel::onUpdateUserList(const QList<User> & users)
{
	beginResetModel();
	users_ = users;
	endResetModel();
}
