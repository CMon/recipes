#include <QCoreApplication>

#include <recipes/common/log.h>
#include <recipes/common/util.h>
#include <recipes/database/password.h>
#include <recipes/server/server.h>

#include <QCommandLineOption>
#include <QCommandLineParser>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(RECIPE_SERVER)
Q_LOGGING_CATEGORY(RECIPE_SERVER, "recipe.server")

/*
 * TODO for server
 *  - Add tests/systemtests
 *  - Check logging, all should log to cateories
 *
 *  - Far future: allow different syncs:
 *    - sync two recipes between two servers
 */

int main(int argc, char ** argv)
{
	QCoreApplication app(argc, argv);
	app.setApplicationName("Receipts Server");
	QCommandLineOption listServiceMethods(QStringList() << "l" << "listServiceMethods", QCoreApplication::translate("main", "List methods that are exported by the services"));
	QCommandLineOption createPassOpt(QStringList() << "p" << "createPass", QCoreApplication::translate("main", "create a has for the given password"), "password");
	QCommandLineOption createPassAskOpt(QStringList() << "P" << "createPassAsk", QCoreApplication::translate("main", "Ask for a password and print its hash"));
	QCommandLineOption logfileOpt(QStringList() << "L" << "log", QCoreApplication::translate("main", "The logfile to log the messages to, if not provided, logging will be done to the console"), "logfile");
	QCommandLineOption logConsoleOpt(QStringList() << "c" << "console", QCoreApplication::translate("main", "Log to console instead of a logfile"));
	QCommandLineParser argParser;
	argParser.addHelpOption();
	argParser.addOption(listServiceMethods);
	argParser.addOption(createPassOpt);
	argParser.addOption(createPassAskOpt);
	argParser.addOption(logfileOpt);
	argParser.addOption(logConsoleOpt);
	argParser.process(app);

	// todo move the filter rules to an option
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

	if (argParser.isSet(createPassOpt)) {
		QTextStream out(stdout);

		const QString password = argParser.value(createPassOpt);
		QString usedCrypto;
		const QByteArray passHash = Password::hashPassword(password, usedCrypto);
		out << "Hash for insertion into the db:" << endl;
		out << "hash: " << passHash << endl << endl;
		out << "passCrypto: " << usedCrypto << endl << endl;
		return 0;
	}

	if (argParser.isSet(createPassAskOpt)) {
		QTextStream out(stdout);
		const QString password = Util::enterPassword("Please enter your password:");
		QString usedCrypto;
		const QByteArray passHash = Password::hashPassword(password, usedCrypto);
		out << "Hash for insertion into the db:" << endl;
		out << "hash: " << passHash << endl << endl;
		out << "passCrypto: " << usedCrypto << endl << endl;
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
