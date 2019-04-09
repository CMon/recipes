#ifndef USERSMODEL_H
#define USERSMODEL_H

#include <recipes/common/user.h>

#include <QAbstractListModel>

class RPCClient;
class UserServiceInterface;

class UsersModel : public QAbstractListModel
{
	Q_OBJECT
public:
	enum AbonementListRoles {
		LoginRole = Qt::UserRole + 1,
		FirstNameRole,
		LastNameRole,
		PermissionsRole,
		IsDeletedRole
	};
	explicit UsersModel(RPCClient * rpcClient, UserServiceInterface * userService, QObject *parent = nullptr);

	Q_INVOKABLE void update();

	int rowCount(const QModelIndex &) const override;
	QVariant data(const QModelIndex & index, int role) const override;
	QHash<int, QByteArray> roleNames() const override;

private slots:
	void onUpdateUserList(const QList<User> & users);

private:
	RPCClient * rpcClient_;
	UserServiceInterface * userService_;
	QList<User> users_;
};

#endif // USERSMODEL_H
