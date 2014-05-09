#include <QCoreApplication>

#include <services/userservice.h>

#include <cflib/util/log.h>
#include <cflib/db/db.h>
#include <cflib/http/apiserver.h>
#include <cflib/http/server.h>

USE_LOG(LogCat::Etc)

int main(int argc, char ** argv)
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("Receipts Server");

    cflib::util::Log::start("recipes-server.log");
    cflib::db::setParameter("recipes", "recipesRoot", "sql");

    UserService userService;

    cflib::http::ApiServer api;
    api.registerService(&userService);

    const int port = 8080;
    const QHostAddress listenOn = QHostAddress::LocalHost;

    cflib::http::Server serv;
    serv.registerHandler(&api);
    if (!serv.start(port, listenOn)) {
        QTextStream(stderr) << "cannot start HTTP-Server (port already in use)" << endl;
        return 1;
    }
    const QString startedMsg = QString("Started Server. Listening on %1:%2").arg(listenOn.toString()).arg(port);
    logInfo(qPrintable(startedMsg));
    QTextStream(stdout) << startedMsg << endl;

    int retval = a.exec();
    logInfo("terminating softly with retval: %1", retval);
    return retval;
}
