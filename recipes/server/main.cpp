#include <QCoreApplication>

#include <recipes/common/log.h>
#include <recipes/server/server.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QLoggingCategory>

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

	Server server;
	server.init();

	if (argParser.isSet(listServiceMethods)) {
		for (const QPair<QString,QString> & method: server.availableServiceMethods()) {
			qDebug() << method.first << method.second;
		}
		return 0;
	}

	qDebug () << "Checking database version";
	if (!server.updateDatabase())
	{
		qCritical() << "Could not update database to newest version, aborting";
		return -1;
	}

	qDebug () << "Starting server";
	if (!server.start()) {
		qCritical() << "Could not start server";
		return -1;
	}

	return app.exec();
}
