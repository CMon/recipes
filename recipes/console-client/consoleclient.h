#pragma once

#include <recipes/console-client/console.h>
#include <recipes/console-client/remote.h>

class QString;
class QStringList;

class ConsoleClient : public QObject
{
	Q_OBJECT
public:
	ConsoleClient(QObject *parent);

	void start();

private slots:
	void command(const QString & command, const QStringList & arguments);

private:
	Console console_;
	Remote  remote_;
};
