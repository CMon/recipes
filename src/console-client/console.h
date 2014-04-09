#pragma once

#include <QObject>
#include <QTextStream>

class QStringList;

typedef QPair<QString, QStringList> CommandAndArguments;

class Console : public QObject
{
    Q_OBJECT
public:
    explicit Console(QObject *parent = 0);

    void start();

signals:
    void commandEntered(const QString & command, const QStringList & arguments);
    void exitConsole();

private:
    CommandAndArguments getNextCommand();
    bool handleInternalCommands(const CommandAndArguments &commandAndArguments);
    void checkCommand(const CommandAndArguments & commandAndArguments);

private:
    QString ps1_;
    QTextStream out_;
    QTextStream in_;

    QList<CommandAndArguments> history_;
};
