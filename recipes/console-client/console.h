#pragma once

#include <QObject>
#include <QStringList>
#include <QTextStream>

class CommandAndArguments
{
public:
	CommandAndArguments() {}
	CommandAndArguments(const QString & command, const QStringList & arguments)
	    : command_(command), arguments_(arguments) {}

	QString toString() const { return command_ + " " + arguments_.join(" "); }
	QString command() const { return command_; }
	QStringList arguments() const { return arguments_; }

private:
	QString command_;
	QStringList arguments_;
};

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

	QList<CommandAndArguments> history_;
};
