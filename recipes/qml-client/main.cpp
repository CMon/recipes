#include <recipes/qml-client/clientcontroller.h>
#include <recipes/qml-client/logcategory.h>
#include <recipes/qml-client/userserviceinterface.h>

#include <rpclib/client/rpcclient.h>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

static QScopedPointer<RPCClient> Client(new RPCClient()); // has to be global otherwise the singleton is unable to get it

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	app.setApplicationName("Recipe gui client");
	qInstallMessageHandler(RecipeLog::consoleMessageHandler);

	qCDebug(GUI_CLIENT) << "Started" << QCoreApplication::applicationName();

	UserServiceInterface userService(Client.data());

	QQmlApplicationEngine engine;
	qmlRegisterSingletonType<ClientController>("CMon.ClientController", 1, 0, "ClientController", [](QQmlEngine *engine, QJSEngine *scriptEngine) -> QObject * {
		Q_UNUSED(engine)
		Q_UNUSED(scriptEngine)
		return new ClientController(Client.data());
	});
	engine.rootContext()->setContextProperty("UserService", &userService);

	engine.load(QUrl(QStringLiteral("qrc:/main.qml")));

	return app.exec();
}
