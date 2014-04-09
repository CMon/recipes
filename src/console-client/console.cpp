#include "console.h"

#include <QStringList>

namespace {
const QString ExitCommand = "exit";
const QString HelpCommand = "help";
const QString HistoryCommand = "history";
const QString ClearHistoryCommand = "clear";
}

Console::Console(QObject *parent) :
    QObject(parent),
    ps1_(" > "),
    out_(stdout),
    in_(stdin)
{
}

void Console::start()
{
    CommandAndArguments commandAndArguments;
    while (commandAndArguments.first.toLower() != ExitCommand) {
        commandAndArguments = getNextCommand();

        checkCommand(commandAndArguments);
    }
    emit exitConsole();
}

CommandAndArguments Console::getNextCommand()
{
    out_ << endl << ps1_;
    out_.flush();

    QStringList enteredValues = in_.readLine().split(" ");

    CommandAndArguments commandAndArguments;
    commandAndArguments.first = enteredValues.takeFirst();
    commandAndArguments.second = enteredValues;

    history_ << commandAndArguments;

    return commandAndArguments;
}

bool Console::handleInternalCommands(const CommandAndArguments & commandAndArguments)
{
    const QString command = commandAndArguments.first;
    const QStringList arguments = commandAndArguments.second;

    bool internalyHandled = false;
    if (command == HistoryCommand) {
       internalyHandled = true;
       if (arguments.isEmpty()) {
           out_ << "History:" << endl;
           int i = 0;
           foreach (const CommandAndArguments & caa, history_) {
               out_ << "[" << i++ << "] " << caa.first << " " << caa.second.join(" ") << endl;
           }
       } else {
           const QString firstArgument = arguments.first();
           bool ok;
           int number = firstArgument.toInt(&ok);
           if (ok && (0 <= number && number < history_.size())) {
               checkCommand(history_.at(number));
           } else if (firstArgument == ClearHistoryCommand) {
               history_.clear();
               out_ << tr("History cleared") << endl;
           } else {
               out_ << tr("Invalid input, for infomation type help") << endl;
           }
       }
    } else if (command == HelpCommand) {
        internalyHandled = true;
        out_
                << "Help:" << endl
                << "  exit              exit console" << endl
                << "  history [number]  show history of entered commands, when number is given, execute this command" << endl
                << "  history clear     clear history" << endl
                   ;
    } else if (command == ExitCommand) {
        internalyHandled = true;
    }

    return internalyHandled;
}

void Console::checkCommand(const CommandAndArguments & commandAndArguments)
{
    if (!handleInternalCommands(commandAndArguments)) {
        emit commandEntered(commandAndArguments.first, commandAndArguments.second);
    }
}
