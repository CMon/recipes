#include <recipes/qml-client/servicemapper.h>
#include <recipes/qml-client/logcategory.h>

#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Receipts gui client");
	qInstallMessageHandler(RecipeLog::consoleMessageHandler);

	qCDebug(GUI_CLIENT) << "Started" << QCoreApplication::applicationName();

	UserService userService;

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
	engine.rootContext()->setContextProperty("UserService", &userService);

	return app.exec();
}
