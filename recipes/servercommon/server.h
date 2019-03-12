#ifndef SERVER_H
#define SERVER_H

#include <recipes/services/recipeservice.h>
#include <recipes/services/userservice.h>
#include <rpclib/server/rpcserver.h>

#include <QHostAddress>
#include <QPointer>
#include <QString>
#include <QUrl>

class Server
{
public:
	Server(const QHostAddress & listenOn = QHostAddress::Any, quint16 port = 8080);

	void init();
	QList<QPair<QString, QString> > availableServiceMethods();

	void setDbCredentials(const QString & databaseName, const QString & user, const QString & password);
	bool updateDatabase();

	bool insertTestData();

	bool start();
	void stop();
	bool isRunning() const;

	QUrl getUrl() const;

private:
	void loadSettings();

private:
	RPCServer rpcServer_;
	QString dbName_;
	QString dbUser_;
	QString dbPass_;

	QPointer<UserService> userService_;
	QPointer<RecipeService> recipeService_;
};

#endif // SERVER_H
