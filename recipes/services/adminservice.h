#pragma once

#include <recipes/common/permissiondata.h>

class RPCServer;
class QWebSocket;

class AdminService : public QObject
{
public:
	AdminService(QObject *parent = nullptr);

	void registerMethods(RPCServer * server);

public: // rpc methods
	QList<PermissionData> getPermissions(QWebSocket * sendingSocket);
	bool addPermission(QWebSocket * sendingSocket, const QString & techName, const Locale2String & title, const Locale2String & description);
	bool setPermissionTitle(QWebSocket * sendingSocket, const int permissionId, const Locale2String & title);
	bool setPermissionDescription(QWebSocket * sendingSocket, const int permissionId, const Locale2String & description);
};
