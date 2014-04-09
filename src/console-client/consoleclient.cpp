#include "consoleclient.h"

#include <common/log.h>

#include <QStringList>
#include <QCoreApplication>

ConsoleClient::ConsoleClient(QObject *parent)
    : QObject(parent),
      console_(this)
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
    LOG_INFO("new command: '%1' with arguments: '%2'", command, arguments.join(", "));
}
