#pragma once

#include <QtMsgHandler>

class Log
{
public:
	static void consoleMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};
