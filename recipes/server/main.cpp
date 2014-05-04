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

    cflib::http::Server serv;
    serv.registerHandler(&api);
    if (!serv.start(8080)) {
        QTextStream(stderr) << "cannot start HTTP-Server (port already in use)" << endl;
        return 1;
    }

    int retval = a.exec();
    logInfo("terminating softly with retval: %1", retval);
    return retval;
}
