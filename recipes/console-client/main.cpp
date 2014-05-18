#include <cflib/util/log.h>
#include <console-client/consoleclient.h>

#include <QCoreApplication>

USE_LOG(LogCat::User)

int main(int argc, char *argv[])
{
	QCoreApplication a(argc, argv);
	a.setApplicationName("Receipts console client");

	ConsoleClient cc(0);
	logInfo("Started %1", QCoreApplication::applicationName());
	cc.start();

	return 0;//a.exec();
}
