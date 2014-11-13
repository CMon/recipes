#include "consoleclient.h"

#include <console-client/logCategory.h>
#include <console-client/remote.h>

#include <QStringList>
#include <QCoreApplication>

ConsoleClient::ConsoleClient(QObject *parent)
    : QObject(parent),
      console_(this),
      remote_(this)
{
	connect(&console_, &Console::exitConsole, QCoreApplication::instance(), &QCoreApplication::quit);
	connect(&console_, &Console::commandEntered, this, &ConsoleClient::command);
}

void ConsoleClient::start()
{
	console_.start();
}

void ConsoleClient::command(const QString &command, const QStringList &arguments)
{
	if (command == "connect") {
		switch (arguments.count()) {
			case 1:
				remote_.connectTo(arguments.first().toInt());
				break;
			case 2:
				remote_.connectTo(arguments.last().toInt(), QHostAddress(arguments.first()));
				break;
			default:
				remote_.connectTo(8080);
				break;
		}
		printf("Connecting to %s\n", qPrintable(remote_.getBaseUrl()));
	} else if (command == "rmi") {
		if (remote_.getBaseUrl().isEmpty()) {
			qCWarning(CONSOLE_CLIENT) << "You need to connect first";
			return;
		}
		switch (arguments.count()) {
			case 1: {
				const QString service = arguments.first();
				const QStringList methods = remote_.getMethodSignatures(service);
				printf("Service: %s\n", qPrintable(service));
				foreach (const QString & method, methods) {
					printf("  %s\n", qPrintable(method));
				}
				break;
			}
			default: {
				const QStringList services = remote_.getServices();
				printf("Services:\n");
				foreach (const QString & service, services) {
					printf("  %s\n", qPrintable(service));
				}
				break;
			}
		}
	} else {
		qCWarning(CONSOLE_CLIENT) << "unknown command:" << command << "with arguments:" << arguments.join(", ");
	}
}
