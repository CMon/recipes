#pragma once

#include <recipes/common/permissiondata.h>

#include <QObject>

class RPCClient;
class User;

class AdminServiceInterface : public QObject
{
	Q_OBJECT
public:
	AdminServiceInterface(RPCClient * rpcClient, QObject *parent = nullptr);

	Q_INVOKABLE void getPermissions() const;
	Q_INVOKABLE void addPermission(const QString & techName, const Locale2String & title, const Locale2String & description) const;
	Q_INVOKABLE void setPermissionTitle(const int permissionId, const Locale2String & title) const;
	Q_INVOKABLE void setPermissionDescription(const int permissionId, const Locale2String & description) const;

	QList<PermissionData> getCachedPermissions() const;

signals:
	void permissionsUpdated();
	void addPermissionSuccesful(bool success);
	void addPermissionTitleSuccesful(bool success);
	void addPermissionDescriptionSuccesful(bool success);

private slots:
	void handleMessageFromServer(const QVariantHash & map);

private:
	void handleCallbackResponse(const QString & signature, const std::string & string);
	void handleNotificationResponse(const QString & notification, const std::string & string);

private:
	QList<PermissionData> permissions_;
	RPCClient * rpcClient_;
};
