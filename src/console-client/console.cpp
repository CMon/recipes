#include "console.h"

#include <common/log.h>
#include <common/util.h>

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
    out_(stdout)
{
}

void Console::start()
{
    CommandAndArguments commandAndArguments;
    while (commandAndArguments.command().toLower() != ExitCommand) {
        commandAndArguments = getNextCommand();

        checkCommand(commandAndArguments);
    }
    emit exitConsole();
}

static void enableConsole(bool enable)
{
    Util::enableConsoleBufferedIo(enable);
    Util::enableConsoleEcho(enable);
}

CommandAndArguments Console::getNextCommand()
{
    out_ << endl << ps1_;
    out_.flush();

    char c = '\0';

    QString string;
    bool endOfLine = false;

    enableConsole(false);
    int currentHistoryIndex = history_.size();
    do {
        c = getchar();

        const int origStringLen = string.size();
        if (c == '\n' || c == '\r') {
            endOfLine = true;
        } else if (c == 0x1b) { // escape char
            char escape1 = getchar();
            char escape2 = getchar();
            if (escape1 == '[') {
                if (escape2 == 'A') {
                    // up
                    if (!history_.isEmpty()) {
                        currentHistoryIndex = qMax(currentHistoryIndex - 1, 0);
                        string = history_.at(currentHistoryIndex).toString();
                    }
                } else if (escape2 == 'B') {
                    // down
                    currentHistoryIndex = qMin(currentHistoryIndex + 1, history_.size());
                    if (currentHistoryIndex >= history_.size()) {
                        string = "";
                    } else {
                        string = history_.at(currentHistoryIndex).toString();
                    }
                } else {
                    LOG_INFO("Unhandled escape2: %1", escape2);
                }
            } else {
                LOG_INFO("Unhandled escape1: '%1'", escape1);
            }
            c ='\0';
        } else if (c == 0x7f) {
            if (string.size() > 0) {
                c = '\b';
                string.chop(1);
            }
        } else {
            string.append(c);
        }

        // write new string
        for(int i = 0; i < origStringLen; ++i) printf("\b \b");
        printf("%s", qPrintable(string));
    } while (!endOfLine);
    printf("\n");
    enableConsole(true);

    QStringList enteredValues = string.split(" ");

    const QString command = enteredValues.takeFirst();
    const CommandAndArguments commandAndArguments(command, enteredValues);

    history_ << commandAndArguments;

    return commandAndArguments;
}

bool Console::handleInternalCommands(const CommandAndArguments & commandAndArguments)
{
    const QString command = commandAndArguments.command();
    const QStringList arguments = commandAndArguments.arguments();

    bool internalyHandled = false;
    if (command == HistoryCommand) {
       internalyHandled = true;
       if (arguments.isEmpty()) {
           out_ << "History:" << endl;
           int i = 0;
           foreach (const CommandAndArguments & caa, history_) {
               out_ << "[" << i++ << "] " << caa.command() << " " << caa.arguments().join(" ") << endl;
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
        emit commandEntered(commandAndArguments.command(), commandAndArguments.arguments());
    }
}
