#include <QCoreApplication>

#include <recipes/common/log.h>
#include <recipes/servercommon/clientinfocache.h>
#include <recipes/services/userservice.h>
#include <recipes/services/recipeservice.h>
#include <recipes/database/database.h>

#include <rpclib/server/rpcserver.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QLoggingCategory>
#include <QHostAddress>

Q_DECLARE_LOGGING_CATEGORY(RECIPE_SERVER)
Q_LOGGING_CATEGORY(RECIPE_SERVER, "recipe.server")

int main(int argc, char ** argv)
{
	QCoreApplication app(argc, argv);
	app.setApplicationName("Receipts Server");
	QCommandLineOption listServiceMethods(QStringList() << "l" << "listServiceMethods", QCoreApplication::translate("main", "List methods that are exported by the services"));
	QCommandLineParser argParser;
	argParser.addHelpOption();
	argParser.addOption(listServiceMethods);
	argParser.process(app);

	qInstallMessageHandler(RecipeLog::consoleMessageHandler);

	QLoggingCategory::setFilterRules(QStringLiteral("recipe.server.*=true"));

	Database::setCredentials("recipes", "root", "sql");

	const int port = 8080;
	const QHostAddress listenOn = QHostAddress::Any;
	RPCServer rpcServer(listenOn, port);

	UserService userService;
	userService.registerMethods(&rpcServer);
	RecipeService recipeService;
	recipeService.registerMethods(&rpcServer);

	if (argParser.isSet(listServiceMethods)) {
		for (const QPair<QString,QString> & method: rpcServer.availableServiceMethods()) {
			qDebug() << method.first << method.second;
		}
		return 0;
	}

	qDebug () << "Starting server";
	if (!rpcServer.start()) {
		qCritical() << "Could not start server";
	}

	return app.exec();
}
