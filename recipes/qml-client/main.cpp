#include <recipes/qml-client/clientcontroller.h>
#include <recipes/qml-client/logcategory.h>
#include <recipes/qml-client/recipesearchmodel.h>
#include <recipes/qml-client/userserviceinterface.h>

#include <rpclib/client/rpcclient.h>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>

static QScopedPointer<RPCClient> Client(new RPCClient()); // has to be global otherwise the singleton is unable to get it

/*
 * TODO for the client:
 *  - Search in Ingredients, recipe titles and step descriptions
 *  - Add Administration functions for the admin: User creation, User "deletion"
 *  - Add a testuser that has all premissions within the testdata.sql. Give the admin a better password
 *  - Add "Add recipe" logic
 *  - Add "Change recipe" logic
 *  - On disconnected from server, do not autoreconnect, but send the client back to the login page
 *  - Hide add/change/admin buttons depending on user permissions
 */

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

	RecipeSearchModel recipeSearchModel(Client.data());

	engine.rootContext()->setContextProperty("recipeSearchModel", &recipeSearchModel);

	engine.load(QUrl(QStringLiteral("qrc:/qml/main.qml")));

	return app.exec();
}
