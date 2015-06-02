#include <QCoreApplication>

#include <recipes/common/log.h>
#include <recipes/services/userservice.h>

#include <QLoggingCategory>
#include <QHostAddress>

#include <cflib/db/db.h>
#include <cflib/net/apiserver.h>
#include <cflib/net/httpserver.h>

Q_DECLARE_LOGGING_CATEGORY(RECIPE_SERVER)
Q_LOGGING_CATEGORY(RECIPE_SERVER, "recipe.server")

int main(int argc, char ** argv)
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("Receipts Server");
	qInstallMessageHandler(Log::consoleMessageHandler);

	QLoggingCategory::setFilterRules(QStringLiteral("recipe.server.*=true"));

	cflib::db::setParameter("recipes", "root", "sql");

	UserService userService;

	cflib::net::ApiServer api;
	api.registerService(&userService);

	const int port = 8080;
	const QHostAddress listenOn = QHostAddress::LocalHost;

	cflib::net::HttpServer serv;
	serv.registerHandler(&api);
	if (!serv.start(port, listenOn.toString().toLatin1())) {
		qCCritical(RECIPE_SERVER) << "cannot start HTTP-Server (port already in use)";
		return 1;
	}
	const QString startedMsg = QString("Started Server. Listening on %1:%2").arg(listenOn.toString()).arg(port);
	qCDebug(RECIPE_SERVER) << startedMsg;
	QTextStream(stdout) << startedMsg << endl;

	int retval = a.exec();
	qCDebug(RECIPE_SERVER) << "terminating softly with retval: " << retval;
	return retval;
}
