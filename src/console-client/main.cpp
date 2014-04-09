#include <common/log.h>
#include <console-client/consoleclient.h>

#include <QCoreApplication>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    a.setApplicationName("Receipts console client");

    ConsoleClient cc(0);
    LOG_NOTICE("Started " + QCoreApplication::applicationName());
    cc.start();

    return 0;//a.exec();
}
