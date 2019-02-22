#pragma once

#include <QtMsgHandler>

class RecipeLog
{
public:
	static bool initFile(const QString & filename);

	static void consoleMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
	static void fileMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg);
};
