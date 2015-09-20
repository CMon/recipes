#include <recipes/qml-client/logcategory.h>
#include <QApplication>
#include <QQmlApplicationEngine>

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Receipts gui client");
	qInstallMessageHandler(RecipeLog::consoleMessageHandler);

	qCDebug(GUI_CLIENT) << "Started" << QCoreApplication::applicationName();

	QQmlApplicationEngine engine;
	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
