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
	QCommandLineOption logfileOpt(QStringList() << "L" << "log", QCoreApplication::translate("main", "The logfile to log the messages to, if not provided, logging will be done to the console"), "logfile");
	QCommandLineOption logConsoleOpt(QStringList() << "c" << "console", QCoreApplication::translate("main", "Log to console instead of a logfile"));
	QCommandLineParser argParser;
	argParser.addHelpOption();
	argParser.addOption(listServiceMethods);
	argParser.addOption(logfileOpt);
	argParser.addOption(logConsoleOpt);
	argParser.process(app);


	QLoggingCategory::setFilterRules(QStringLiteral("recipe.server.*=true"));

	if (argParser.isSet(logConsoleOpt)) {
		qInstallMessageHandler(RecipeLog::consoleMessageHandler);
	} else if (argParser.isSet(logfileOpt)) {
		RecipeLog::initFile(argParser.value(logfileOpt));
		qInstallMessageHandler(RecipeLog::fileMessageHandler);
	} else {
		RecipeLog::initFile(QString("%1.log").arg(argv[0]));
		qInstallMessageHandler(RecipeLog::fileMessageHandler);
	}
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
